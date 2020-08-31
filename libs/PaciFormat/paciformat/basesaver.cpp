#include "basesaver.h"

namespace paci {


BaseSaver::BaseSaver(Sequence* sequence, QUrl saveUrl, QObject *parent)
  : BaseFormatOperator(parent),
    m_sequence(sequence),
    m_saveUrl(saveUrl) {}


void BaseSaver::doWork()
{
  emit started();
  showMessage(tr("Doing nothing...:)"));
  emit finished(m_hasError);
}



} // namespace paci
