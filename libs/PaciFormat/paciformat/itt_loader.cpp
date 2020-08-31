#include "itt_loader.h"

#include <QFileInfo>

namespace paci {

ITTLoader::ITTLoader(QUrl fileUrl, QObject* parent)
  : BaseLoader(fileUrl, parent) {
  m_config.setDefaults(
    { { "defaultRate", 24 }, { "defaultDropframe", false } });
}

QString ITTLoader::parseSequenceName() {
  QFileInfo info(sourceUrl().toLocalFile());
  return info.completeBaseName();
}

Timebase ITTLoader::parseTimebase() {
  int rate = m_config.get<int>("defaultRate");
  bool dropframe = m_config.get<bool>("defaultDropframe");
  return Timebase(rate, dropframe);
}

TrackManager* ITTLoader::parseTrackManager() {
  auto defualt_tb = parseTimebase();
  auto* manager = new TrackManager;
  auto elems = mainDoc()
                 .documentElement()
                 .firstChildElement("body")
                 .firstChildElement("div")
                 .elementsByTagName("p");
  if (elems.isEmpty()) {
    setHasError(true);
    showMessage(tr("文件中没有任何字幕"));
  }
  for (int i = 0; i < elems.length(); ++i) {
    if (m_canceled)
      break;
    auto e = elems.item(i).toElement();
    auto start_s = e.attribute("begin");
    auto end_s = e.attribute("end");
    auto new_clip = new Clip(e.text());
    new_clip->setStartTime(
      TimePoint::fromTimecode(start_s, defualt_tb));
    new_clip->setEndTime(TimePoint::fromTimecode(end_s, defualt_tb));
    new_clip->setParent(this);
    manager->autoInsertClip(new_clip);
  }
  return manager;
}

QDomDocument& ITTLoader::mainDoc() {
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

QStringList ITTLoader::previewLines() {
  QStringList result;
  auto elems = mainDoc()
                 .documentElement()
                 .firstChildElement("body")
                 .firstChildElement("div")
                 .elementsByTagName("p");
  for (int i = 0; i < elems.length(); ++i) {
    if (result.length() >= 20)
      break;
    result << elems.item(i).toElement().text();
  }
  return result;
}

} // namespace paci
