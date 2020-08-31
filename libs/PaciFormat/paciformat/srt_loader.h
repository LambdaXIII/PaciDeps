#ifndef SRT_LOADER_H
#define SRT_LOADER_H

#include "baseloader.h"
#include "paciformat_global.h"

#include <QObject>
#include <optional>

namespace paci {

class SRTLoader
    :public BaseLoader
{
  Q_OBJECT
public:
  explicit SRTLoader(QUrl fileUrl, QObject *parent=nullptr);

protected:
  using CaptionPack = QStringList;
  QString parseSequenceName() override;
  TrackManager * parseTrackManager() override;
  std::optional<QList<CaptionPack>> m_sourceData;
  QList<CaptionPack> sourceData();

  std::pair<TimePoint,TimePoint> parseTimes(const CaptionPack &p);
  QString parseContent(const CaptionPack &p);
};


}//ns paci

#endif // SRT_LOADER_H
