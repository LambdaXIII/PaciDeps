#ifndef PACIBASEOPERATOR_H
#define PACIBASEOPERATOR_H

#include "property_macros.h"

#include <QObject>

namespace paci {
class PaciBaseOperator: public QObject {
  Q_OBJECT

  PA_WRITABLE_PROPERTY(bool, hasError, HasError)
  PA_READONLY_PROPERTY(bool, canceled, Canceled)
  PA_READONLY_PROPERTY(int, totalProgress, TotalProgress)
  PA_READONLY_PROPERTY(int, currentProgress, CurrentProgress)
  PA_READONLY_PROPERTY(QString, currentMessage, CurrentMessage)

public:
  explicit PaciBaseOperator(QObject* parent = nullptr);

protected:
  void showMessage(const QString& msg);

signals:
  void workCanceled();
  //  void started();
  //  void finished();

public slots:
  void cancelWork();
};
} // namespace paci
#endif // PACIBASEOPERATOR_H
