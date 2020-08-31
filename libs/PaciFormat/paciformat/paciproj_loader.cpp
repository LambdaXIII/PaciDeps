#include "paciproj_loader.h"

#include "warlock/numbertools.h"

#include <QDebug>
#include <QDomNodeList>

namespace paci {

PaciProjLoader::PaciProjLoader(QUrl fileUrl, QObject* parent)
  : BaseLoader(fileUrl, parent) {
}

Sequence* PaciProjLoader::doWork() {
  updateTotalProgress(1);
  updateCurrentProgress(0);

  emit started();
  paci::Sequence* sequence;

  QFile input(m_sourceUrl.toLocalFile());
  if (! input.open(TEXTROMODE)) {
    sequence = new paci::Sequence(tr("错误序列"));
    sequence->setParent(this);
    sequence->setNote(tr("序列文件无法打开"));
    setHasError(true);
    showMessage(tr("源文件无法打开"));
  } else {
    QDomDocument doc("source_project");
    doc.setContent(&input);
    QDomElement root = doc.documentElement();
    if (root.tagName() != proj::SEQ)
      throw std::invalid_argument(
        tr("源文件根结点名称为：%1,应该是：%2")
          .arg(root.tagName())
          .arg(proj::SEQ)
          .toStdString());

    // sequence name & note
    QString sequence_name =
      root.firstChildElement(proj::SEQNAME).text();
    showMessage(sequence_name);
    QString sequence_note =
      root.firstChildElement(proj::SEQNOTE).text();
    // sequence timebase
    int rate = root.firstChildElement(proj::TIMEBASE)
                 .attribute(proj::TB_RATE)
                 .toInt();
    bool df = proj::bool_value_from(
      root.firstChildElement(proj::TIMEBASE).attribute(proj::TB_DF));
    Timebase sequence_timebase(rate, df);
    // sequence global font
    QDomElement fontElement = root.firstChildElement(proj::FONT);
    auto* sequence_fontInfo = parseFontInfo(fontElement);
    showMessage(tr("全局字体导入完毕"));
    // track manager
    auto* sequence_trackManager = new TrackManager(this);
    QDomElement trackManagerElement =
      root.firstChildElement(proj::TRACKMANAGER);
    QDomNodeList trackNodes = trackManagerElement.childNodes();
    if (! trackNodes.isEmpty())
      for (int i = 0; i < trackNodes.length(); ++i) {
        if (m_canceled)
          break;
        auto trackNode = trackNodes.item(i).toElement();
        if (trackNode.tagName() == proj::TRACK) {
          auto* track = parseTrack(trackNodes.item(i).toElement());
          track->setParent(sequence_trackManager);
          sequence_trackManager->appendTrack(track);
        }
      }

    sequence = new Sequence(sequence_name, sequence_timebase,
      sequence_fontInfo, sequence_trackManager, sequence_note, this);

    auto extra_data_nodes = root.childNodes();
    if (! extra_data_nodes.isEmpty())
      for (int i = 0; i < extra_data_nodes.length(); ++i) {
        auto ede = extra_data_nodes.item(i).toElement();
        if (ede.tagName() == proj::EXTRA_DATA)
          sequence->setExtraProperty(
            ede.attribute(proj::EXTRA_DATA_NAME),
            ede.attribute(proj::EXTRA_DATA_VALUE));
      }
  }

  input.close();
  emit resultReady(sequence, m_hasError);
  return sequence;
}

Track* PaciProjLoader::parseTrack(const QDomElement& elem) {
  if (elem.tagName() != proj::TRACK)
    throw std::invalid_argument(
      tr("传入了一个轨道节点，但是标签并不是：%1")
        .arg(proj::TRACK)
        .toStdString());

  auto* result = new Track(this);
  // track tag
  auto track_tag = elem.attribute(proj::TRACK_TAG);
  showMessage(tr("开始导入轨道：%1").arg(track_tag));
  result->setTag(track_tag);
  // track activated
  auto track_act =
    proj::bool_value_from(elem.attribute(proj::ACTIVATED));
  showMessage(track_act ? tr("轨道已启用") : tr("轨道未启用"));
  result->setIsActivated(track_act);
  // track font switch
  auto track_fontSwitch =
    proj::bool_value_from(elem.attribute(proj::TRACKFONTSWITCH, "NO"));
  showMessage(
    track_fontSwitch ? tr("轨道字体启用") : tr("轨道字体禁用"));
  result->setFontSwitch(track_fontSwitch);
  // track font info
  auto fontElem = elem.firstChildElement(proj::FONT);
  if (! fontElem.isNull()) {
    auto* font = parseFontInfo(elem.firstChildElement(proj::FONT));
    showMessage(tr("轨道%1字体设置导入完毕")
                  .arg(elem.attribute(proj::TRACK_TAG)));
    result->fontInfo()->copyPropertiesFrom(font);
    font->deleteLater();
  }
  // parse clips
  QDomNodeList clipNodes = elem.childNodes();
  if (! clipNodes.isEmpty()) {
    addTotalProgress(clipNodes.length());
    for (int i = 0; i < clipNodes.length(); ++i) {
      if (m_canceled)
        break;
      QDomElement e = clipNodes.item(i).toElement();
      if (e.tagName() == proj::CLIP) {
        auto* c =
          new Clip("", TimePoint(e.attribute(proj::STARTTIME).toInt()),
            TimePoint(e.attribute(proj::DURATION).toInt()), result);
        c->setIsActivated(
          proj::bool_value_from(e.attribute(proj::ACTIVATED)));
        auto unimark = e.attribute(proj::UNIMARK, "").simplified();
        if (! unimark.isEmpty())
          c->setUniqueMark(unimark.toStdString());

        if (! e.text().isEmpty())
          c->setContent(e.text());

        auto sub_es = e.childNodes();
        for (int i = 0; i < sub_es.length(); ++i) {
          auto se = sub_es.item(i).toElement();
          if (se.tagName() == proj::CONTENT)
            c->setContent(se.text());
          else if (se.tagName() == proj::EXTRA_DATA)
            c->setExtraProperty(se.attribute(proj::EXTRA_DATA_NAME),
              se.attribute(proj::EXTRA_DATA_VALUE));
        }

        result->append(c);
      } // parse clip

      if (e.tagName() == proj::EXTRA_DATA) {
        result->setExtraProperty(e.attribute(proj::EXTRA_DATA_NAME),
          e.attribute(proj::EXTRA_DATA_VALUE));
      } // parse extra data

      currentProgressPlusOne();
    }
  }
  return result;
}

FontInfo* PaciProjLoader::parseFontInfo(const QDomElement& elem) {
  if (elem.tagName() != proj::FONT)
    throw std::invalid_argument(
      tr("导入了一个字体节点，但是标签是：%1，应该是：%2")
        .arg(elem.tagName())
        .arg(proj::FONT)
        .toStdString());

  auto* result = new FontInfo(this);
  QDomNodeList children = elem.childNodes();
  if (! children.isEmpty())
    for (int i = 0; i < children.length(); ++i) {
      QDomElement e = children.item(i).toElement();
      QString tag = e.tagName();
      if (proj::FONT_KEYS_NUMBER.contains(tag)) {
        result->setValue(tag, e.text().toDouble());
      } else if (proj::FONT_KEYS_BOOL.contains(tag)) {
        result->setValue(tag, proj::bool_value_from(e.text()));
      } else if (proj::FONT_KEYS_COLOR.contains(tag)) {
        QStringList a { proj::RED, proj::GREEN, proj::BLUE,
          proj::ALPHA };
        QList<double> colors;
        for (auto& x : a)
          colors << e.attribute(x).toDouble();
        Color c(colors[0], colors[1], colors[2], colors[3]);
        result->setValue(tag, c);
      } else {
        result->setValue(tag, e.text());
      }
    }

  return result;
}

QStringList PaciProjLoader::previewLines() {
  QStringList result;
  QDomDocument doc(proj::SEQ);
  QFile xml(m_sourceUrl.toLocalFile());
  if (xml.open(TEXTROMODE)) {
    doc.setContent(&xml);
    QDomElement root = doc.documentElement();
    auto elems = root.elementsByTagName(proj::CLIP);
    int m = warlock::smaller(20, elems.size());
    for (int i = 0; i < m; i++) {
      auto line = elems.item(i).toElement().text();
      result << line;
    }
    xml.close();
  } else {
    result << tr("预览读取失败");
  }
  return result;
}

} // namespace paci
