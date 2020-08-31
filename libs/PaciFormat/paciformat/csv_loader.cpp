#include "csv_loader.h"

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
  /*
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
*/

  csv::Reader reader;
  reader.configure_dialect("excel")
    .delimiter(m_config.get("comma").toString().toStdString())
    .skip_initial_space("true")
    .skip_empty_rows(true)
    .header(false);

  reader.read(m_sourceUrl.toLocalFile().toStdString());

  bool load_extra_columns = m_config.get("loadExtraColumns").toBool();

  bool passed_header = ! m_config.get("hasHeader").toBool();

  while (reader.busy()) {
    if (reader.ready()) {
      auto row = reader.next_row();
      if (! passed_header) {
        passed_header = true;
        continue;
      }

      //读取行数据
      QStringList row_data;
      for (const auto& pair : row)
        row_data << QString::fromStdString(pair.second);
      if (row_data.count() < 2)
        continue;

      //处理行数据
      TimePoint start, end;
      QString start_s = row_data.takeFirst();
      auto end_s = row_data.takeFirst();
      switch (m_config.get("timeMode").toInt()) {
      case MillSecond:
        start = TimePoint(start_s.toInt());
        end = TimePoint(end_s.toInt());
        break;
      case Second:
        start = TimePoint::fromSeconds(start_s.toDouble());
        end = TimePoint::fromSeconds(end_s.toDouble());
        break;
      case Timecode:
        start = TimePoint::fromTimecode(start_s, parseTimebase());
        end = TimePoint::fromTimecode(end_s, parseTimebase());
        break;
      }
      //读取数据
      if (row_data.isEmpty()) {
        auto* clip = new Clip("", start);
        clip->setEndTime(end);
        tm->autoInsertClip(clip);
      } else
        for (int i = 0; i < row_data.length(); ++i) {
          if (i > 0 && (! load_extra_columns))
            break;
          auto* clip = new Clip(row_data.at(i), start);
          clip->setEndTime(end);
          tm->autoInsertClip(clip);
        }
    }
  }

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
