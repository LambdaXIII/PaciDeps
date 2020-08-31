//
// Created by 张珺超 on 2019/10/27.
//

#ifndef PACIONE_BASIC_OBJECT_H
#define PACIONE_BASIC_OBJECT_H

#include "pacicore_global.h"
#include "pacibase/property_macros.h"

#include <QMap>
#include <QObject>

namespace paci {

class PACICORESHARED_EXPORT BasicObject: public QObject {
  Q_OBJECT

  PA_WRITABLE_PROPERTY(bool, isActivated, IsActivated)

public:
  explicit BasicObject(QObject* p = nullptr);

  Q_INVOKABLE QString extraProperty(const QString& name) const;
  Q_INVOKABLE void setExtraProperty(
    const QString& name, const QString& value);
  Q_INVOKABLE void clearExtraProperties();
  Q_INVOKABLE QStringList extraPropertyKeys() const;

protected:
  QMap<QString, QString> m_extraProperties;
};
} // namespace paci

#endif // PACIONE_BASIC_OBJECT_H
