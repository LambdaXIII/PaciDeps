#include "fcp7xml_loader.h"

#include "pacicore/clip.h"
#include "warlock/hashstring.h"

namespace paci {

Fcp7XMLLoader::Fcp7XMLLoader(QUrl fileUrl, QObject* parent)
  : BaseLoader(fileUrl, parent) {
  m_config.setDefaults({ { "readUUID", false } });
}

QDomDocument& Fcp7XMLLoader::mainDoc() {
  if (! m_mainDoc) {
    QFile file(m_sourceUrl.toLocalFile());
    if (! file.open(TEXTROMODE))
      throw tr("源文件读取错误");
    QDomDocument doc;
    doc.setContent(&file);
    m_mainDoc = doc;
    file.close();
  }
  return *m_mainDoc;
}

QString Fcp7XMLLoader::parseSequenceName() {
  return mainDoc()
    .documentElement()
    .firstChildElement("sequence")
    .firstChildElement("name")
    .text();
}

Timebase Fcp7XMLLoader::parseTimebase() {
  auto e = mainDoc()
             .documentElement()
             .firstChildElement("sequence")
             .firstChildElement("rate")
             .toElement();
  int r = e.firstChildElement("timebase").text().toInt();
  bool df = e.firstChildElement("ntsc").text() == "TRUE";
  m_timebaseCache = Timebase(r, df);
  return m_timebaseCache;
}

QString Fcp7XMLLoader::paraType(const QDomElement& e) {
  QString result = e.tagName();
  auto id_node = e.firstChildElement("parameterid");
  if (! id_node.isNull()) {
    //确保有返回值，如果节点没得内容，则返回节点标签。
    result = id_node.toElement().text();
  }

  return result;
}

QString Fcp7XMLLoader::paraValue(const QDomElement& e) {
  QString result = e.text();
  auto value_node = e.lastChildElement("value");
  if ((! value_node.isNull()) && (e.tagName() == "parameter")) {
    result = value_node.toElement().text().simplified();
  }
  return result;
}

void Fcp7XMLLoader::setupFont(FontInfo* font, const QDomElement& elem) {
  showMessage(tr("正在解析字体信息"));

  auto fxID = elem.firstChildElement("effectid").toElement().text();
  if (fxID == "Outline Text" || fxID == "Text") {
    auto para_nodes = elem.childNodes();
    addTotalProgress(para_nodes.length());
    for (int k = 0; k < para_nodes.length(); k++) {
      QDomNode node = para_nodes.item(k);
      auto t = paraType(node.toElement());
      auto v = paraType(node.toElement());

#define SS(x) warlock::hash_const(x)
      switch (warlock::hash_switch(t.toStdString().c_str())) {
      case SS("fontname"):
      case SS("font"):
        font->setFamily(v);
      case SS("fontsize"):
      case SS("size"):
        font->setSize(v.toDouble());
      case SS("fontalign"):
      case SS("align"): {
        QMap<QString, FontInfo::Alignment> alignMap {
          { "1", FontInfo::Left }, { "2", FontInfo::Center },
          { "3", FontInfo::Right }
        };
        font->setAlignment(alignMap[v]);
      }
      case SS("fonttrack"):
      case SS("track"):
        font->setTrack(v.toDouble());
      case SS("leading"):
      case SS("lead"):
        font->setLead(v.toDouble());
      case SS("aspect"):
        font->setAspect(v.toDouble());
      case SS("opacity"):
        font->setOpacity(v.toInt());
      case SS("fontcolor"):
      case SS("textolor"):
        font->setColor(parseColor(node.toElement()));
      case SS("linecolor"):
        font->setOutlineColor(parseColor(node.toElement()));
      case SS("linewidth"):
        font->setOutlineWidth(v.toDouble());
      case SS("linesoft"):
        font->setOutlineSoft(v.toDouble());
      } // switch
#undef SS
      currentProgressPlusOne();
    } // for
  }

  showMessage(tr("字体信息解析完毕"));
}

Color Fcp7XMLLoader::parseColor(const QDomElement& elem) {
  auto v = elem.lastChildElement("value");
  return Color::from255Value(
    v.firstChildElement("red").toElement().text().toInt(),
    v.firstChildElement("green").toElement().text().toInt(),
    v.firstChildElement("blue").toElement().text().toInt(),
    v.firstChildElement("alpha").toElement().text().toInt());
}

bool Fcp7XMLLoader::isTextClip(const QDomElement& e) {
  auto idns =
    e.elementsByTagName("effect").at(0).toElement().elementsByTagName(
      "effectid");
  if (! idns.isEmpty()) {
    auto id = idns.item(0).toElement().text();
    if (id == "Outline Text" || id == "Text")
      return true;
  }
  return false;
}

Track* Fcp7XMLLoader::parseTrack(const QDomElement& elem) {
  auto track = new Track;
  auto clip_nodes = elem.elementsByTagName("generatoritem");
  addTotalProgress(clip_nodes.length());

  for (int i = 0; i < clip_nodes.length(); i++) {
    auto clip_e = clip_nodes.at(i).toElement();
    if (isTextClip(clip_e)) {
      int startF = clip_e.firstChildElement("start").text().toInt();
      int endF = clip_e.firstChildElement("end").text().toInt();

      bool enabled = true;
      auto direct_childs = clip_e.childNodes();
      for (int i = 0; i < direct_childs.length(); ++i) {
        auto enabled_e = direct_childs.item(i).toElement();
        if (enabled_e.tagName() == "enabled"
            && enabled_e.text().simplified().toUpper() == "FALSE") {
          enabled = false;
          break;
        }
      }

      QString content;
      auto params = clip_e.elementsByTagName("parameter");
      for (int j = 0; j < params.length(); j++) {
        auto p = params.item(j);
        if (paraType(p.toElement()) == "str") {
          content = paraValue(p.toElement());
          break;
        }
      }

      auto clip = new Clip(content);
      auto tb = parseTimebase();
      clip->setStartTime(TimePoint::fromFrames(startF, tb));
      clip->setEndTime(TimePoint::fromFrames(endF, tb));
      clip->setIsActivated(enabled);

      if (m_config.get("readUUID").toBool()) {
        auto uuid_e = clip_e.firstChildElement("uuid");
        if (! uuid_e.isNull())
          clip->setUniqueMark(uuid_e.text().toStdString());
      }

      if (! isClipSkipped(*clip)) {
        clip->setParent(track);
        track->append(clip);
      }

      showMessage(content);
    }
    currentProgressPlusOne();
  }

  return track;
}

TrackManager* Fcp7XMLLoader::parseTrackManager() {
  auto* manager = new TrackManager;
  auto trackElems = mainDoc()
                      .documentElement()
                      .firstChildElement("sequence")
                      .firstChildElement("media")
                      .firstChildElement("video")
                      .elementsByTagName("track");
  for (int i = 0; i < trackElems.length(); i++) {
    auto* track = parseTrack(trackElems.at(i).toElement());
    track->setTag(QString::number(manager->size() + 1));
    manager->appendTrack(track);
  }
  return manager;
}

Sequence* Fcp7XMLLoader::doWork() {
  emit started();
  auto res = new Sequence(parseSequenceName(),
    parseTimebase(),
    parseSequenceFontInfo(),
    parseTrackManager(),
    parseNote());
  res->fontInfo()->copyPropertiesFrom(
    res->trackManager()->at(0)->fontInfo());
  emit resultReady(res, m_hasError);
  return res;
}

QStringList Fcp7XMLLoader::previewLines() {
  QStringList results;
  auto clipElems = mainDoc()
                     .documentElement()
                     .firstChildElement("sequence")
                     .firstChildElement("media")
                     .firstChildElement("video")
                     .elementsByTagName("generatoritem");
  for (int i = 0; i < clipElems.length(); i++) {
    if (results.length() >= 20)
      break;
    auto clip_e = clipElems.at(i).toElement();
    if (isTextClip(clip_e)) {
      QString content;
      auto params = clip_e.elementsByTagName("parameter");
      for (int j = 0; j < params.length(); j++) {
        auto p = params.item(j);
        if (paraType(p.toElement()) == "str") {
          content = paraValue(p.toElement());
          break;
        }
      }
      results << content;
    }
  }
  return results;
}

} // namespace paci
