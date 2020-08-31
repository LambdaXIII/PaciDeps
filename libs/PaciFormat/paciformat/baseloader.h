#ifndef BASELOADER_H
#define BASELOADER_H

#include <QFile>
#include <QObject>
//#include "basicmacros.h"
#include "baseformatoperator.h"
#include "paciformat_global.h"
#include "pacicore/sequence.h"

#include <QUrl>

namespace paci {

class PACIFORMATSHARED_EXPORT BaseLoader: public BaseFormatOperator {
  Q_OBJECT
  Q_PROPERTY(QUrl sourceUrl READ sourceUrl CONSTANT)

protected:
  QUrl m_sourceUrl;

  virtual QString parseSequenceName();
  virtual Timebase parseTimebase();
  virtual FontInfo* parseSequenceFontInfo();
  virtual QString parseNote();
  virtual TrackManager* parseTrackManager();

public:
  explicit BaseLoader(QUrl fileUrl, QObject* parent = nullptr);

  Q_INVOKABLE QUrl sourceUrl() const;

signals:
  void started();
  void resultReady(Sequence* resultSequence, bool error = false);

public slots:
  virtual Q_INVOKABLE QStringList previewLines();
  virtual Q_INVOKABLE Sequence* doWork();
};

} // namespace paci

#endif // BASELOADER_H
