#ifndef SRT_SAVER_H
#define SRT_SAVER_H

#include "basesaver.h"
#include "paciformat_global.h"

#include <QObject>

namespace paci{
class SRTSaver
    :public BaseSaver
{
  Q_OBJECT
public:
  explicit SRTSaver(Sequence* sequence, QUrl saveUrl, QObject* parent = nullptr);

protected:
  const static QString m_recordTemplate;
  int m_currentRecordNumber{0};
  QString forgeRecord(const paci::Clip& clip) const;
  QString trackSavePath(int index) const;

  void saveTrack(const Track &track,const QString &path);
  void saveSingle();

public slots:
  void doWork() override;
};


}//ns paci
#endif // SRT_SAVER_H
