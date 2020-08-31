#include "srt_loader.h"

#include <QFileInfo>
#include <QStringList>

namespace paci {

SRTLoader::SRTLoader(QUrl fileUrl, QObject* parent)
  : BaseLoader(fileUrl, parent) {
}

QString paci::SRTLoader::parseSequenceName() {
  QFileInfo info(m_sourceUrl.toLocalFile());
  return info.completeBaseName();
}

TrackManager* SRTLoader::parseTrackManager() {
  auto data = sourceData();
  auto* result = new TrackManager;

  for (const auto& p : data) {
    auto times = parseTimes(p);
    auto c = parseContent(p);
    showMessage(c);
    auto* clip = new Clip(c);
    clip->setStartTime(times.first);
    clip->setEndTime(times.second);
    if (! isClipSkipped(*clip))
      result->autoInsertClip(clip);
    currentProgressPlusOne();
  }

  return result;
}

QList<SRTLoader::CaptionPack> SRTLoader::sourceData() {
  if (! m_sourceData.has_value()) {
    m_sourceData = QList<CaptionPack>();
    QFile in(m_sourceUrl.toLocalFile());
    if (in.open(TEXTROMODE)) {
      QTextStream s(&in);

      CaptionPack p;
      while (! s.atEnd()) {
        auto a = s.readLine();
        if (a.isEmpty() && (! p.isEmpty())) {
          *m_sourceData << p;
          p.clear();
        } else {
          p << a;
        }
      }
      in.close();

    } else {
      setHasError(true);
      showMessage(tr("文件读取失败"));
    }
  }
  return *m_sourceData;
}

std::pair<TimePoint, TimePoint> SRTLoader::parseTimes(
  const SRTLoader::CaptionPack& p) {
  auto line = p[1];
  auto parts = line.split("-->");
  auto time1 = TimePoint::fromTimestamp(parts[0].simplified());
  auto time2 = TimePoint::fromTimestamp(parts[1].simplified());
  return std::make_pair(time1, time2);
}

QString SRTLoader::parseContent(const SRTLoader::CaptionPack& p) {
  if (p.length() < 3)
    return "";
  QStringList a;
  for (int i = 2; i < p.length(); ++i) {
    a << p[i];
  }
  return a.join("\n");
}

} // namespace paci
