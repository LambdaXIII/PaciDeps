#ifndef PACIFORTUNE_H
#define PACIFORTUNE_H
#include "pacifortune_global.h"

#include <QObject>

class PACIFORTUNESHARED_EXPORT PaciFortune: public QObject {
  Q_OBJECT
public:
  explicit PaciFortune(QObject* p = nullptr);
  Q_INVOKABLE QString pickOne() const;
  QStringList& fortunes();

protected:
  QStringList m_fortunes;
  static const QString NORMAL_FORTUNES;
};

#endif // PACIFORTUNE_H
