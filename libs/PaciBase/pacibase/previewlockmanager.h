#ifndef PREVIEWLOCKMANAGER_H
#define PREVIEWLOCKMANAGER_H

#include <QDate>
#include <QObject>

namespace paci {

class PreviewLockManager: public QObject {
  Q_OBJECT
public:
  explicit PreviewLockManager(QObject* parent = nullptr);

  Q_INVOKABLE static bool isPreviewLockEnabled();

  Q_INVOKABLE static QDate compileDate();

  Q_INVOKABLE QDate deadlineDate() const;

  Q_INVOKABLE bool isOutdated() const;

protected:
  const static int m_preview_days;
signals:
};

} // namespace paci

#endif // PREVIEWLOCKMANAGER_H
