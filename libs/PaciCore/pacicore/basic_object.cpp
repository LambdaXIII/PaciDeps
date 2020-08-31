//
// Created by 张珺超 on 2019/10/27.
//

#include "basic_object.h"

namespace paci {

BasicObject::BasicObject(QObject* p)
  : QObject(p)
  , m_isActivated(true) {
}

QString BasicObject::extraProperty(const QString& name) const {
  if (m_extraProperties.keys().contains(name))
    return m_extraProperties[name];
  return "";
}

void BasicObject::setExtraProperty(
  const QString& name, const QString& value) {
  if (! name.isEmpty()) {
    if (! value.isEmpty())
      m_extraProperties[name] = value;
    else if (m_extraProperties.contains(name))
      m_extraProperties.remove(name);
  }
}

void BasicObject::clearExtraProperties() {
  m_extraProperties.clear();
}

QStringList BasicObject::extraPropertyKeys() const {
  return m_extraProperties.keys();
}

} // namespace paci
