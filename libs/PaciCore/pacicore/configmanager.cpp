#include "configmanager.h"

#include "pacibase/padebug.h"
//#include "overload.h"

namespace paci {

void ConfigManager::setDefaults(
  std::initializer_list<std::pair<QString, QVariant>> init_list) {
  for (const auto& pair : init_list)
    setDefault(pair.first, pair.second);
}

void ConfigManager::setDefault(
  const QString& name, const QVariant& value) {
  m_defaultMap[name] = value;
}

void ConfigManager::set(const QString& name, const QVariant& value) {
  m_valueMap[name] = value;
}

void ConfigManager::sets(
  std::initializer_list<std::pair<QString, QVariant>> init_list) {
  for (const auto& pair : init_list)
    set(pair.first, pair.second);
}

QVariant ConfigManager::get(const QString& name) const {
  return m_valueMap.value(name, m_defaultMap.value(name, QVariant()));
}

QVariant ConfigManager::getDefault(const QString& name) const {
  return m_defaultMap.value(name, QVariant());
}

QSet<QString> ConfigManager::keys() const {
  QSet<QString> res;
  for (const auto& k : m_defaultMap.keys())
    res << k;
  for (const auto& k : m_valueMap.keys())
    res << k;
  return res;
}

void ConfigManager::clear() {
  m_valueMap.clear();
}

void ConfigManager::saveValueToDefualt() {
  for (const auto& v : m_valueMap.keys()) {
    m_defaultMap[v] = m_valueMap[v];
  }
  m_valueMap.clear();
}

void ConfigManager::addOptions(const ConfigManager& other) {
  auto keys = other.keys();
  for (const auto& k : keys) {
    set(k, other.get(k));
  }
}

QDebug operator<<(QDebug dbg, const ConfigManager& mng) {
  dbg.nospace() << "def_map:" << mng.m_defaultMap << "\n";
  dbg.nospace() << "val_map:" << mng.m_valueMap;
  return dbg;
}

} // namespace paci
