#ifndef PACIPROJ_LOADER_H
#define PACIPROJ_LOADER_H

#include "baseloader.h"
#include "paciformat_global.h"
#include "paciproj_common.h"

#include <QObject>
#include <QXmlStreamReader>
//#include <optional>
#include <QDomElement>

namespace paci {

class PACIFORMATSHARED_EXPORT PaciProjLoader: public BaseLoader {
  Q_OBJECT

public:
  explicit PaciProjLoader(QUrl fileUrl, QObject* parent = nullptr);

  Q_INVOKABLE Sequence* doWork() override;
  Q_INVOKABLE QStringList previewLines() override;

protected:
  FontInfo* parseFontInfo(const QDomElement& elem);
  Track* parseTrack(const QDomElement& elem);
};

} // namespace paci

#endif // PACIPROJ_LOADER_H
