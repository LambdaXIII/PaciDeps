#ifndef FLAMEXML_LOADER_H
#define FLAMEXML_LOADER_H

#include "baseloader.h"
#include "paciformat_global.h"

#include <QDomDocument>
#include <QDomElement>
#include <QObject>
#include <optional>

namespace paci{
class FlameXMLLoader
    :public BaseLoader
{
  Q_OBJECT
public:
  explicit FlameXMLLoader(QUrl fileUrl, QObject* parent = nullptr);
protected:
  QString parseSequenceName() override;
  Timebase parseTimebase() override;
  TrackManager* parseTrackManager() override;
  FontInfo * parseSequenceFontInfo() override;

  std::optional<QDomDocument> m_mainDoc;
  QDomDocument& mainDoc();
  std::optional<Timebase> m_timebase;
public:
  Q_INVOKABLE QStringList previewLines() override;
};
}//ns paci
#endif // FLAMEXML_LOADER_H
