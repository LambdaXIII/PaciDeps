#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include "baseloader.h"
#include "csv/reader.hpp"
#include "paciformat_global.h"

#include <QObject>
namespace paci {
class PACIFORMATSHARED_EXPORT CSVLoader: public BaseLoader {
  Q_OBJECT
public:
  enum TimeMode { MillSecond = 0, Second, Timecode };
  Q_ENUM(TimeMode)

  explicit CSVLoader(QUrl fileUrl, QObject* parent = nullptr);

  QString parseSequenceName() override;
  Timebase parseTimebase() override;
  TrackManager* parseTrackManager() override;
  QStringList previewLines() override;

protected:
  TimePoint parseTime(QString time);
};
} // namespace paci
#endif // CSV_LOADER_H
