#include "pacibaseoperator.h"

namespace paci {
PaciBaseOperator::PaciBaseOperator(QObject* parent)
  : QObject(parent)
  , m_hasError(false)
  , m_canceled(false)
  , m_totalProgress(100)
  , m_currentProgress(0) {
}

void PaciBaseOperator::cancelWork() {
  m_canceled = true;
  showMessage(tr("任务取消。"));
  emit workCanceled();
}

void PaciBaseOperator::showMessage(const QString& msg) {
  if (msg != m_currentMessage) {
    m_currentMessage = msg;
    emit currentMessageChanged(msg);
  }
}
} // namespace paci
