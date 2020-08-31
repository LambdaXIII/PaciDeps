#include "csv_loader.h"

#include "qtcsv/reader.h"
#include "qtcsv/stringdata.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <string>

namespace paci {

CSVLoader::CSVLoader(QUrl fileUrl, QObject* parent)
  : BaseLoader(fileUrl, parent) {
  m_config.setDefaults(
    { { "hasHeader", false }, { "comma", QString(",") },
      { "loadExtraColumns", true }, // 读取更多的列加入其它轨道
      { "defaultRate", 24 }, { "defaultDropframe", false },
      { "timeMode", MillSecond } });
}

QString CSVLoader::parseSequenceName() {
  QFileInfo info(m_sourceUrl.toLocalFile());
  return info.completeBaseName();
}

Timebase CSVLoader::parseTimebase() {
  return Timebase(m_config.get<int>("defaultRate"),
    m_config.get<bool>("defaultDropframe"));
}

TrackManager* CSVLoader::parseTrackManager() {
  //  const char comma =
  //  m_config.get<QString>("comma").at(0).toLatin1();
  updateTotalProgress(100);
  updateCurrentProgress(0);
  TrackManager* tm = new TrackManager();

  showMessage(tr("读取数据中…"));
  QtCSV::StringData data;
  QtCSV::Reader::readToData(
    m_sourceUrl.toLocalFile(), data, m_config.get<QString>("comma"));
  // TODO:多种编码支持
  updateCurrentProgress(30);
  showMessage(tr("数据解析中…"));
  if (m_config.get<bool>("hasHeader"))
    data.removeRow(0);
  for (int row = 0; row < data.rowCount(); ++row) {
    auto line = data.rowValues(row);
    if (line.length() < 3) {
      showMessage(tr("跳过无效行"));
      continue;
    }
    auto line_0 = line.takeFirst();
    auto line_1 = line.takeFirst();

    TimePoint startTime = parseTime(line_0);
    TimePoint endTime = parseTime(line_1);

    for (const auto& cont : line) {
      auto a = new Clip(cont);
      showMessage(cont);
      a->setStartTime(startTime);
      a->setEndTime(endTime);
      tm->autoInsertClip(a);
      // FIXME: 可能会有轨道顺序问题
    }
  }

  updateCurrentProgress(90);

  return tm;
}

QStringList CSVLoader::previewLines() {
  QStringList result;
  // TODO: 还没写呢
  return result;
}

TimePoint CSVLoader::parseTime(QString time) {
  int time_mode = m_config.get<int>("timeMode");
  switch (time_mode) {
  case MillSecond:
    return TimePoint(time.toInt());
  case Second:
    return TimePoint::fromSeconds(time.toDouble());
  case Timecode:
    return TimePoint::fromTimecode(time, parseTimebase());
  }
  return TimePoint(0);
}

} // namespace paci
