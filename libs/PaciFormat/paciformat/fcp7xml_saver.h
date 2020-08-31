#ifndef FCP7XMLSAVER_H
#define FCP7XMLSAVER_H

#include "basesaver.h"
#include "paciformat_global.h"

#include <QObject>
//#include "basicmacros.h"
#include <QDomDocument>
#include <QDomElement>

namespace paci {
class PACIFORMATSHARED_EXPORT Fcp7XMLSaver: public BaseSaver {
  Q_OBJECT
public:
  enum TextMode { PlainText, OutlineText };
  Q_ENUM(TextMode)

  explicit Fcp7XMLSaver(
    Sequence* sequence, QUrl saveUrl, QObject* parent = nullptr);

protected:
  const static QString tree_rc;
  const static QString plain_rc;
  const static QString outline_rc;
  static QString fillBlanks(
    QString& ori, QMap<QString, QString>& missions);
  static QMap<QString, QString> fontMap(const FontInfo* font);

  QString treeString() const;
  QDomElement clipElem(const Clip& clip, const FontInfo* font);
  QDomElement trackElem(const Track* track);

public slots:
  void doWork() override;
};
} // namespace paci
#endif // FCP7XMLSAVER_H
