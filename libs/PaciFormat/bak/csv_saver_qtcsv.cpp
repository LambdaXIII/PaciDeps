#include "csv_saver.h"

#include "qtcsv/stringdata.h"
#include "qtcsv/writer.h"

#include <QDataStream>
#include <QFileInfo>
#include <QSaveFile>
#include <string>

namespace paci {

CSVSaver::CSVSaver(Sequence* sequence, QUrl saveUrl, QObject* parent)
  : BaseSaver(sequence, saveUrl, parent) {
  m_config.setDefaults(
    { { "hasHeader", true }, { "comma", QString(",") } });

  m_headers << tr("起始点（时间码）") << tr("片段时长（时间码）")
            << tr("结束点（时间码）") << tr("起始点（帧数）")
            << tr("片段时长（帧数）") << tr("结束点（帧数）")
            << tr("起始点（秒）") << tr("片段时长（秒）")
            << tr("结束点（秒）") << tr("片段内容") << tr("片段识别码");
}

QString CSVSaver::pathForTrack(int index) {
  paci::Track* track = m_sequence->trackManager()->at(index);
  QFileInfo info(m_saveUrl.toLocalFile());
  auto path = info.path();
  auto basename = info.completeBaseName();
  auto suffix = info.suffix();
  auto extraName =
    tr("_轨道%1_%2")
      .arg(index + 1)
      .arg(track->tag().isEmpty() ? tr("无标签") : track->tag());
  return path + "/" + basename + extraName + "." + suffix;
}

void CSVSaver::saveTrack(Track* track, const QString& path) {
  showMessage(tr("开始准备数据"));
  QtCSV::StringData strData;
  if (m_config.get<bool>("hasHeader"))
    strData.addRow(m_headers);

  for (const auto& c : *track) {
    showMessage(c->content());
    QStringList c_data;
    c_data << c->startTime().toTimecode(m_sequence->timebase())
           << c->durationTime().toTimecode(m_sequence->timebase())
           << c->endTime().toTimecode(m_sequence->timebase());
    c_data << QString::number(
      c->startTime().toFrames(m_sequence->timebase()))
           << QString::number(
                c->durationTime().toFrames(m_sequence->timebase()))
           << QString::number(
                c->endTime().toFrames(m_sequence->timebase()));
    c_data << QString::number(c->startTime().toSeconds())
           << QString::number(c->durationTime().toSeconds())
           << QString::number(c->endTime().toSeconds());
    c_data << c->content() << QString::fromStdString(c->uniqueMark());
    strData.addRow(c_data);
    currentProgressPlusOne();
  }

  QSaveFile saveFile(path);
  showMessage(tr("开始写入文件：%1").arg(path));

  if (! saveFile.open(TEXTWOMODE)) {
    showMessage(tr("文件写入失败"));
    setHasError(true);
    saveFile.cancelWriting();
    throw tr("无法打开目标文件");
  }

  QtCSV::Writer::write(
    saveFile, strData, m_config.get<QString>("comma"));
  // TODO: 更多编码支持
  saveFile.flush();

  if (m_canceled) {
    saveFile.cancelWriting();
    showMessage(tr("用户取消了操作"));
    return;
  }

  saveFile.commit();
}

void CSVSaver::doWork() {
  updateCurrentProgress(0);
  updateTotalProgress(m_sequence->trackManager()->clipCount());
  emit started();
  showMessage(tr("每个轨道将会拆分成单独的文件"));

  auto* manager = m_sequence->trackManager();
  for (int i = 0; i < manager->length(); ++i) {
    if (m_canceled)
      break;
    auto* track = manager->at(i);
    if (! isTrackSkipped(track)) {
      auto outPath = pathForTrack(i);

      saveTrack(track, outPath);
    }
  }
  updateCurrentProgress(totalProgress());
  showMessage(tr("搞完了"));
  emit finished(m_hasError);
}

} // namespace paci
