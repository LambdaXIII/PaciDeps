#ifndef ITT_LOADER_H
#define ITT_LOADER_H
#include "baseloader.h"
#include "paciformat_global.h"

#include <QDomDocument>
#include <QDomElement>
#include <QObject>
#include <optional>

namespace paci {

class ITTLoader: public BaseLoader {
  Q_OBJECT
public:
  explicit ITTLoader(QUrl fileUrl, QObject* parent = nullptr);

protected:
  QString parseSequenceName() override;
  Timebase parseTimebase() override;
  TrackManager* parseTrackManager() override;

  std::optional<QDomDocument> m_mainDoc;
  QDomDocument& mainDoc();
public slots:
  Q_INVOKABLE QStringList previewLines() override;
};
} // namespace paci
#endif // ITT_LOADER_H
