#include "srt_saver.h"

#include "warlock/logicpool.h"

#include <QFileInfo>
#include <QSaveFile>
#include <QTextStream>

namespace paci {
SRTSaver::SRTSaver(Sequence* sequence, QUrl saveUrl, QObject* parent)
  : BaseSaver(sequence, saveUrl, parent) {
  m_config.setDefaults(
    { { "premiereCompatible", false }, { "singleFile", false } });
}

const QString SRTSaver::m_recordTemplate = { "%2%1%3 --> %4%1%5%1%1" };

QString SRTSaver::forgeRecord(const Clip& clip) const {
  QString new_line =
    m_config.get<bool>("premiereCompatible") ? "\r\n" : "\n";
  auto start_ts = clip.startTime().toTimestamp();
  auto end_ts = clip.endTime().toTimestamp();

  QString record = m_recordTemplate.arg(new_line)
                     .arg(m_currentRecordNumber)
                     .arg(start_ts)
                     .arg(end_ts)
                     .arg(clip.content());
  return record;
}

QString SRTSaver::trackSavePath(int index) const {
  QFileInfo o(m_saveUrl.toLocalFile());
  auto* track = m_sequence->trackManager()->at(index);
  return o.absolutePath() + "/" + o.completeBaseName()
         + QString("_%1_%2").arg(index).arg(
           track->tag().isEmpty() ? tr("无标签") : track->tag())
         + "." + o.suffix();
}

void SRTSaver::saveTrack(const Track& track, const QString& path) {
  bool pr_mode = m_config.get<bool>("premiereCompatible");

  QString target_path = path;
  QSaveFile out(target_path);
  if (! out.open(TEXTWOMODE)) {
    setHasError(true);
    out.cancelWriting();
    return;
  }
  if (pr_mode)
    out.putChar(QChar(QChar::ByteOrderMark).toLatin1());
  m_currentRecordNumber = 1;
  for (const auto* clip : track) {
    if (m_canceled || isClipSkipped(*clip))
      continue;
    auto r = forgeRecord(*clip);
    out.write(r.toUtf8());
    m_currentRecordNumber++;
    currentProgressPlusOne();
  }
  out.commit();
}

void SRTSaver::saveSingle() {
  bool pr_mode = m_config.get<bool>("premiereCompatible");
  QSaveFile out(m_saveUrl.toLocalFile());
  if (! out.open(TEXTWOMODE)) {
    setHasError(true);
    out.cancelWriting();
    return;
  }
  if (pr_mode)
    out.putChar(QChar(QChar::ByteOrderMark).toLatin1());

  m_currentRecordNumber = 0;
  for (const auto* track : *m_sequence->trackManager())
    for (const auto* clip : *track) {
      if (m_canceled)
        out.cancelWriting();
      warlock::LogicPool logic { isTrackSkipped(track),
        isClipSkipped(*clip), m_canceled };
      if (logic.anyRight())
        continue;
      auto r = forgeRecord(*clip);
      out.write(r.toUtf8());
      m_currentRecordNumber++;
      currentProgressPlusOne();
    }
  //  if(m_hasError) out.cancelWriting();
  out.commit();
}

void SRTSaver::doWork() {
  updateTotalProgress(m_sequence->trackManager()->clipCount());
  updateCurrentProgress(0);
  bool single_mode = m_config.get<bool>("singleFile");

  emit started();
  if (single_mode) {
    saveSingle();
  } else {
    for (int i = 0; i < m_sequence->trackManager()->length(); ++i) {
      auto* track = m_sequence->trackManager()->at(i);
      if (! isTrackSkipped(track))
        saveTrack(*track, trackSavePath(i));
    }
  }

  emit finished(m_hasError);
}

} // namespace paci
