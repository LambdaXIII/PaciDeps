#ifndef TEXT_LOADER_H
#define TEXT_LOADER_H

#include <QObject>
#include "paciformat_global.h"
#include "baseloader.h"

namespace paci {
class TextLoader
  : public BaseLoader
{
  Q_OBJECT

public:
  explicit TextLoader(QUrl fileUrl, QObject *p = nullptr);

  Q_INVOKABLE TrackManager* parseTrackManager() override;
  Q_INVOKABLE Timebase parseTimebase() override;
  Q_INVOKABLE QStringList previewLines() override;
  Q_INVOKABLE QString parseSequenceName() override;

protected:
  int timeOf(const QString &s);
};




}

#endif // TEXT_LOADER_H
