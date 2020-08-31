#include "flamexml_loader.h"

#include <QFileInfo>

namespace paci {

FlameXMLLoader::FlameXMLLoader(QUrl fileUrl, QObject* parent)
  : BaseLoader(fileUrl, parent) {
}

QString FlameXMLLoader::parseSequenceName() {
  QFileInfo info(sourceUrl().toLocalFile());
  return info.completeBaseName();
}

Timebase FlameXMLLoader::parseTimebase() {
  if (! m_timebase) {
    auto elem =
      mainDoc().documentElement().firstChildElement("rate").text();
    bool df = elem.contains(".");
    int rate = qRound(elem.toDouble());
    m_timebase = std::make_optional<Timebase>(rate, df);
  }
  return *m_timebase;
}

TrackManager* FlameXMLLoader::parseTrackManager() {
  auto* manager = new TrackManager;
  auto elems = mainDoc().documentElement().elementsByTagName("title");
  addTotalProgress(elems.length());
  if (elems.isEmpty()) {
    showMessage(tr("文件中没得任何片段"));
    setHasError(true);
  }
  for (int i = 0; i < elems.length(); ++i) {
    if (m_canceled)
      break;
    auto e = elems.item(i).toElement();
    auto start = e.firstChildElement("start").text();
    auto end = e.firstChildElement("end").text();
    auto text = e.firstChildElement("text").text();
    auto new_clip = new Clip(text);
    new_clip->setParent(this);
    new_clip->setStartTime(
      TimePoint::fromTimecode(start, parseTimebase()));
    new_clip->setEndTime(TimePoint::fromTimecode(end, parseTimebase()));
    if (! isClipSkipped(*new_clip))
      manager->autoInsertClip(new_clip);
    currentProgressPlusOne();
  }
  return manager;
}

FontInfo* FlameXMLLoader::parseSequenceFontInfo() {
  auto* font = new FontInfo;
  auto elems = mainDoc().documentElement().elementsByTagName("title");
  if (! elems.isEmpty()) {
    auto elem = elems.item(0).toElement();
    font->setFamily(elem.firstChildElement("font").text());
    font->setSize(elem.firstChildElement("size").text().toDouble());
#define C(channel)                                                     \
  elem.firstChildElement("fontcolor")                                  \
    .firstChildElement(#channel)                                       \
    .text()                                                            \
    .toInt()
    font->setColor(
      Color::from255Value(C(red), C(green), C(blud), C(alpha)));
#undef C
  }

  return font;
}

QDomDocument& FlameXMLLoader::mainDoc() {
  if (! m_mainDoc) {
    QFile file(m_sourceUrl.toLocalFile());
    if (! file.open(TEXTROMODE))
      throw tr("源文件读取错误");
    QDomDocument doc;
    doc.setContent(&file);
    m_mainDoc = doc;
  }
  return *m_mainDoc;
}

QStringList FlameXMLLoader::previewLines() {
  QStringList result;
  auto elems = mainDoc().documentElement().elementsByTagName("title");
  for (int i = 0; i < elems.length(); ++i) {
    if (result.length() >= 20)
      break;
    result << elems.item(i).toElement().text();
  }
  return result;
}

} // namespace paci
