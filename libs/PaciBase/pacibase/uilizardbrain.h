#ifndef UILIZARDBRAIN_H
#define UILIZARDBRAIN_H

#include "previewlockmanager.h"

#include <QCoreApplication>
#include <QDate>
#include <QObject>

namespace paci {

class UILizardBrain: public QObject {
  Q_OBJECT
  Q_CLASSINFO("author", "xiii_1991")
  Q_CLASSINFO("email", "xiii_1991@163.com")
  Q_CLASSINFO("note", "I am a UIBrain.")
  Q_PROPERTY(QString appName READ appName CONSTANT)
  Q_PROPERTY(QString appVersion READ appVersion CONSTANT)
  Q_PROPERTY(QString compileDate READ compileDateStr CONSTANT)
  Q_PROPERTY(bool isOutdated READ isOutdated CONSTANT)
  Q_PROPERTY(bool isPreviewVersion READ isPreviewVersion CONSTANT)

public:
  explicit UILizardBrain(QObject* parent = nullptr);

  static QString appName();
  static QString appVersion();
  QString compileDateStr() const;
  bool isOutdated() const;
  bool isPreviewVersion() const;

protected:
  PreviewLockManager* m_previewLocker;
};

} // namespace paci
#endif // UILIZARDBRAIN_H
