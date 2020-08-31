#include "previewlockmanager.h"

#include "time_lock_defs.h"

namespace paci {

PreviewLockManager::PreviewLockManager(QObject* parent)
  : QObject(parent) {
}

bool PreviewLockManager::isPreviewLockEnabled() {
#ifdef PREVIEW_LOCK
  return true;
#else
  return false;
#endif
}

QDate PreviewLockManager::compileDate() {
  return QDate::fromString(COMPILE_DATE, "yyyy-MM-dd");
}

QDate PreviewLockManager::deadlineDate() const {
  return compileDate().addDays(m_preview_days);
}

bool PreviewLockManager::isOutdated() const {
  auto is_out = QDate::currentDate() > deadlineDate();
  return isPreviewLockEnabled() && is_out;
}

const int PreviewLockManager::m_preview_days = 45;

} // namespace paci
