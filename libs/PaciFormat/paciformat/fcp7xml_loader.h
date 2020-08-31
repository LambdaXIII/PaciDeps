#ifndef FCP7XML_LOADER_H
#define FCP7XML_LOADER_H

#include "baseloader.h"
#include "paciformat_global.h"

#include <QDomDocument>
#include <QDomElement>
#include <QObject>
#include <optional>

namespace paci {
class PACIFORMATSHARED_EXPORT Fcp7XMLLoader: public BaseLoader {
  Q_OBJECT
public:
  explicit Fcp7XMLLoader(QUrl fileUrl, QObject* parent = nullptr);

  Q_INVOKABLE QStringList previewLines() override;

protected:
  std::optional<QDomDocument> m_mainDoc;
  QDomDocument& mainDoc();

  QString parseSequenceName() override;
  Timebase parseTimebase() override;
  TrackManager* parseTrackManager() override;

  void setupFont(FontInfo* font, const QDomElement& elem);

  Track* parseTrack(const QDomElement& elem);

  static Color parseColor(const QDomElement& elem);
  static QString paraType(const QDomElement& e);
  static QString paraValue(const QDomElement& e);
  static bool isTextClip(const QDomElement& e);

  Timebase m_timebaseCache;

public slots:
  Q_INVOKABLE Sequence* doWork() override;
};

} // namespace paci
#endif // FCP7XML_LOADER_H
