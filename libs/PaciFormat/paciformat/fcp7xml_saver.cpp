#include "fcp7xml_saver.h"

#include <QFile>
#include <QSaveFile>
#include <QTextStream>
#include <stdexcept>

namespace paci {

Fcp7XMLSaver::Fcp7XMLSaver(
  Sequence* sequence, QUrl saveUrl, QObject* parent)
  : BaseSaver(sequence, saveUrl, parent) {
  m_config.setDefaults(
    { { "outlineTextMode", true }, { "forceEnableClips", false } });
  // TODO: 添加UUID支持
}

const QString Fcp7XMLSaver::tree_rc = {
  ":/templates/fcp7emptytree.xml"
};
const QString Fcp7XMLSaver::plain_rc = {
  ":/templates/fcp7plaintextclip.xml"
};
const QString Fcp7XMLSaver::outline_rc = {
  ":/templates/fcp7outlinetextclip.xml"
};

QString Fcp7XMLSaver::fillBlanks(
  QString& ori, QMap<QString, QString>& missions) {
  auto o = ori;
  for (auto const& k : missions.keys()) {
    auto realK = QString("{%1}").arg(k);
    o = o.replace(realK, missions[k]);
  }
  return o;
}

QString Fcp7XMLSaver::treeString() const {
  QFile file(tree_rc);
  if (! file.open(TEXTROMODE))
    throw std::invalid_argument(tr("无法打开主序列模板").toStdString());

  QTextStream in(&file);
  QString temp = in.readAll();
  file.close();

  QMap<QString, QString> answers { { "SEQUENCENAME",
                                     m_sequence->sequenceName() },
    { "DURATION", QString::number(m_sequence->durationTime().toFrames(
                    m_sequence->timebase())) },
    { "NTSC", m_sequence->timebase().dropframe ? "TRUE" : "FALSE" },
    { "TIMEBASE", QString::number(m_sequence->timebase().rate) },
    { "TIMEBASESTRING", m_sequence->timebase().timeString() } };
  auto res = fillBlanks(temp, answers);
  return res;
}

QMap<QString, QString> Fcp7XMLSaver::fontMap(const FontInfo* font) {
  QMap<QString, QString> map;
  map["FONTNAME"] = font->family();

  if (font->bold())
    map["FONTSTYLE"] = font->italic() ? "4" : "2";
  else
    map["FONTSTYLE"] = font->italic() ? "3" : "1";

  switch (font->alignment()) {
  case FontInfo::Left:
    map["ALIGN"] = "1";
    break;
  case FontInfo::Center:
    map["ALIGN"] = "2";
    break;
  default:
    map["ALIGN"] = "3";
    break;
  }

#define NV(x, y) map[#x] = QString::number(y)

  NV(FONTSIZE, font->size());
  NV(TRACKSPACE, font->track());
  NV(ROWSPACE, font->lead());
  NV(ASPECT, font->aspect());
  NV(OUTLINEWIDTH, font->outlineWidth());
  NV(OUTLINESOFT, font->outlineSoft());
  NV(OPACITY, font->opacity());

  auto t_color = font->color().to255Int();
  NV(COLORRED, t_color[0]);
  NV(COLORGREEN, t_color[1]);
  NV(COLORBLUE, t_color[2]);
  NV(COLORALPHA, t_color[3]);

  auto oc = font->outlineColor().to255Int();
  NV(OUTLINERED, oc[0]);
  NV(OUTLINEGREEN, oc[1]);
  NV(OUTLINEBLUE, oc[2]);
  NV(OUTLINEALPHA, oc[3]);

#undef NV

  return map;
}

QDomElement Fcp7XMLSaver::clipElem(
  const Clip& clip, const FontInfo* font) {
  QFile file(
    m_config.get<bool>("outlineTextMode") ? plain_rc : outline_rc);
  if (! file.open(TEXTROMODE))
    throw std::invalid_argument(tr("无法打开片段模板").toStdString());

  QTextStream in(&file);
  QString temp = in.readAll();
  file.close();

  bool clip_enabled =
    m_config.get<bool>("forceEnableClips") ? true : clip.isActivated();

#define NV(x, y) { #x, QString::number(y) }
  QMap<QString, QString> answers { NV(TEXTDURATION,
                                     clip.durationTime().toFrames(
                                       m_sequence->timebase())),
    { "NTSC", m_sequence->timebase().dropframe ? "TRUE" : "FALSE" },
    NV(TIMEBASE, m_sequence->timebase().rate),
    NV(OUTPOINT,
      clip.durationTime().toFrames(m_sequence->timebase()) + 1000),
    NV(STARTPOINT, clip.startTime().toFrames(m_sequence->timebase())),
    NV(ENDPOINT, clip.endTime().toFrames(m_sequence->timebase())),
    { "TEXTSTRING", clip.content() },
    { "ENABLED", clip_enabled ? "TRUE" : "FALSE" } };
#undef NV

  auto font_map = fontMap(font);
  for (const auto& k : font_map.keys())
    answers[k] = font_map[k];

  auto clipString = fillBlanks(temp, answers);
  QDomDocument doc;
  doc.setContent(clipString);
  return doc.documentElement().toElement();
}

QDomElement Fcp7XMLSaver::trackElem(const Track* track) {
  QDomDocument t;
  QDomElement elem = t.createElement("track");
  if (! track->tag().isEmpty()) {
    auto commet = t.createComment(tr("轨道标签：%1").arg(track->tag()));
    elem.appendChild(commet);
  }

  auto* font =
    track->fontSwitch() ? track->fontInfo() : m_sequence->fontInfo();
  for (Clip* clip : *track) {
    if (m_canceled)
      break;

    if (! isClipSkipped(*clip)) {
      elem.appendChild(clipElem(*clip, font));
      showMessage(clip->content());
      currentProgressPlusOne();
    }
  }

  return elem;
}

void Fcp7XMLSaver::doWork() {
  updateTotalProgress(m_sequence->trackManager()->clipCount());
  updateCurrentProgress(0);
  emit started();
  showMessage(tr("导出开始"));

  auto tree = treeString();
  QDomDocument doc;
  doc.setContent(tree);

  QDomElement doc_root_e = doc.documentElement();
  QDomElement video_e =
    doc_root_e.elementsByTagName("video").item(0).toElement();

  showMessage(tr("开始生成文档"));
  for (const auto* track : *(m_sequence->trackManager())) {
    if (m_canceled)
      break;
    if (! isTrackSkipped(track)) {
      video_e.appendChild(trackElem(track));
    }
  }

  showMessage(tr("开始写入文档"));
  QString xml = doc.toString();
  QStringList xmls = xml.split("\n");
  addTotalProgress(xmls.count());

  QSaveFile out(m_saveUrl.toLocalFile());
  if (! out.open(TEXTWOMODE)) {
    showMessage(tr("目标文件打不开"));
    out.cancelWriting();
    throw tr("无法打开目标文件");
  }

  QTextStream s(&out);
  for (const auto& line : xmls) {
    if (m_canceled)
      break;
    s << line << "\n";
    currentProgressPlusOne();
  }

  if (m_canceled) {
    showMessage(tr("用户取消了操作"));
    out.cancelWriting();
  } else {
    showMessage(tr("文件写入完毕"));
    out.flush();
    out.commit();
  }

  emit finished(m_hasError);
}

} // namespace paci
