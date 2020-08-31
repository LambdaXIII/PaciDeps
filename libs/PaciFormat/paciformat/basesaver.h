#ifndef BASESAVER_H
#define BASESAVER_H

#include "baseformatoperator.h"
#include "paciformat_global.h"
#include "pacibase/property_macros.h"
#include "pacicore/sequence.h"

#include <QIODevice>
#include <QObject>
#include <QUrl>

namespace paci {

class PACIFORMATSHARED_EXPORT BaseSaver: public BaseFormatOperator {
  Q_OBJECT

  PA_CONSTANT_PROPERTY(paci::Sequence*, sequence)
  PA_CONSTANT_PROPERTY(QUrl, saveUrl)

public:
  explicit BaseSaver(
    Sequence* sequence, QUrl saveUrl, QObject* parent = nullptr);

protected:
signals:
  void started();
  void finished(bool error = false);

public slots:
  virtual Q_INVOKABLE void doWork();
};

} // namespace paci

#endif // BASESAVER_H
