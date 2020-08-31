#include "text_loader.h"

#include "warlock/numbertools.h"
//#include <QDebug>
#include "pacibase/coding_detector.h"
#include "pacibase/padebug.h"
#include "pacicore/basicstructs.h"

#include <QFileInfo>
#include <QTextStream>

namespace paci {

TextLoader::TextLoader(QUrl fileUrl, QObject* p)
  : BaseLoader(fileUrl, p) {
  m_config.setDefaults({ { "minimumClipDuration", int(48) },
    { "durationPerChar", int(3) }, { "spaceBetweenClips", int(0) },
    { "defaultTBRate", int(24) }, { "defaultDropframe", false } });
  //  PA_INFO(m_config.dumpInfo())
}

QString TextLoader::parseSequenceName() {
  QFileInfo x(m_sourceUrl.toLocalFile());
  return x.baseName();
}

int TextLoader::timeOf(const QString& s) {
  int fpc = m_config.get<int>("durationPerChar");
  int mcd = m_config.get<int>("minimumClipDuration");
  int len = s.length() * fpc;
  //  qDebug() << m_config.get<int>("durationPerChar");
  return warlock::larger(len, mcd);
}

Timebase TextLoader::parseTimebase() {
  return Timebase(m_config.get<int>("defaultTBRate"),
    m_config.get<bool>("defaultDropframe"));
}

TrackManager* TextLoader::parseTrackManager() {
  auto* tm = new TrackManager();
  tm->appendEmptyTrack();
  Track* t = tm->first();

  QFile file(m_sourceUrl.toLocalFile());
  updateTotalProgress(100);
  updateCurrentProgress(-1);

  CodingDetector cdt;
  QString code_name = cdt.detect(m_sourceUrl.toLocalFile());
  //  qDebug() << code_name;

  if (file.open(QIODevice::ReadOnly)) {
    int nextStartPoint = 0;

    QTextStream ins(&file);

    if (! code_name.isEmpty())
      ins.setCodec(code_name.toStdString().c_str());

    while ((! ins.atEnd()) && (! canceled())) {
      auto line = ins.readLine();
      auto cont = line.simplified();
      if (cont.isEmpty())
        continue;

      int dur = timeOf(cont);

      auto tb = parseTimebase();
      auto* newClip = new Clip(cont,
        TimePoint::fromFrames(nextStartPoint, tb),
        TimePoint::fromFrames(dur, tb));

      nextStartPoint =
        newClip->endTime().toFrames(tb)
        + warlock::positiveGate(m_config.get<int>("spaceBetweenClips"));
      showMessage(newClip->content());
      /*
            PA_INFO(QString("Clip[%1,%2](%3) parsed, next point %4(+
         %5).") .arg(newClip->startTime().toFrames(tb))
                      .arg(newClip->durationTime().toFrames(tb))
                      .arg(newClip->content())
                      .arg(nextStartPoint)
                      .arg(m_config.get<int>("spaceBetweenClips")))
                */

      t->forceAppend(newClip);
      currentProgressPlusOne();
      if (currentProgress() >= totalProgress())
        updateTotalProgress(totalProgress() + 100);
    }
    file.close();
    updateCurrentProgress(totalProgress());
  } else {
    setHasError(true);
    showMessage(tr("文件打开出错"));
  }

  return tm;
}

QStringList TextLoader::previewLines() {
  QStringList res;
  QFile in(m_sourceUrl.toLocalFile());
  if (in.open(TEXTROMODE)) {
    int count = 0;
    QTextStream ins(&in);
    while ((! ins.atEnd()) && (count < 20)) {
      auto line = ins.readLine().simplified();
      if (line.isEmpty())
        continue;
      res << line;
      count++;
    }
  } else {
    res << tr("预览读取出错");
  }
  return res;
}

} // namespace paci
