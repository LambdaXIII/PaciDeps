#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

//#include <QVariantMap>
#include "pacicore_global.h"

//#include <QDebug>
#include <QObject>
#include <QSet>
#include <QString>
#include <QVariantMap>
#include <initializer_list>
#include <utility>

namespace paci {

class PACICORESHARED_EXPORT ConfigManager {
  Q_GADGET
public:
  ConfigManager() = default;

  void setDefaults(
    std::initializer_list<std::pair<QString, QVariant>> init_list);
  void setDefault(const QString& name, const QVariant& value);

  void set(const QString& name, const QVariant& value);
  void sets(
    std::initializer_list<std::pair<QString, QVariant>> init_list);

  QVariant get(const QString& name) const;
  QVariant getDefault(const QString& name) const;

  template <typename T>
  T get(const QString& name) const {
    return get(name).value<T>();
  }

  template <typename T>
  void set(const QString& name, const T& value) {
    m_valueMap.insert(name, QVariant::fromValue<T>(value));
  }

  QSet<QString> keys() const;

  void clear();
  void saveValueToDefualt();

  void addOptions(const ConfigManager& other);

  friend QDebug operator<<(QDebug dbg, const ConfigManager& mng);

protected:
  QVariantMap m_defaultMap, m_valueMap;
};

} // namespace paci

Q_DECLARE_METATYPE(paci::ConfigManager)

#endif // CONFIGMANAGER_H
