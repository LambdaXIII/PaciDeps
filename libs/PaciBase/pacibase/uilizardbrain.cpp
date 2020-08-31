#include "uilizardbrain.h"

namespace paci {

UILizardBrain::UILizardBrain(QObject* parent)
  : QObject(parent)
  , m_previewLocker(new PreviewLockManager(this)) {
}

QString UILizardBrain::appName() {
  return QCoreApplication::applicationName();
}

QString UILizardBrain::appVersion() {
  return QCoreApplication::applicationVersion();
}

QString UILizardBrain::compileDateStr() const {
  //  return
  //  m_previewLocker->compileDate().toString(QLocale::ShortFormat);
  return "";
}

bool UILizardBrain::isOutdated() const {
  return m_previewLocker->isOutdated();
}

bool UILizardBrain::isPreviewVersion() const {
  return m_previewLocker->isPreviewLockEnabled();
}

} // namespace paci
