#ifndef TEXT_SAVER_H
#define TEXT_SAVER_H
#include "basesaver.h"
#include "paciformat_global.h"

#include <QObject>
//#include "basicmacros.h"

namespace paci {

class TextSaver: public BaseSaver {
  Q_OBJECT

public:
  explicit TextSaver(
    Sequence* sequence, QUrl saveUrl, QObject* parent = nullptr);

protected:
  bool isTrackSkipped(const Track* track) const override;

public slots:
  Q_INVOKABLE void doWork() override;
};

} // namespace paci
#endif // TEXT_SAVER_H
