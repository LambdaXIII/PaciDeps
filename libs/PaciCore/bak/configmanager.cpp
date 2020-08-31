#include "configmanager.h"

#include "padebug.h"
//#include "overload.h"

namespace paci {
// ConfigManager::ConfigManager()
//  : SimpleConfigManager() {}

// ConfigManager::ConfigManager(const ConfigManager &other)
//  : SimpleConfigManager(other) {}

// ConfigManager::ConfigManager(std::initializer_list<OptionPair> list)
//  : SimpleConfigManager(list) {}

QString ConfigManager::vInfo(const OptionVariant& v) const {
  if (std::holds_alternative<int>(v)) {
    return QString::number(std::get<int>(v));
  } else if (std::holds_alternative<bool>(v)) {
    return std::get<bool>(v) ? "TRUE" : "FALSE";
  } else if (std::holds_alternative<std::string>(v)) {
    return QString::fromStdString(std::get<std::string>(v));
  }
  return "[UNKNOWN]";
}

std::string ConfigManager::keyToString(const QString& key) const {
  return key.toStdString();
}

QString ConfigManager::dumpInfo() const {
  QString lineTemplate("%1 = %2 | %3\n");

  QStringList res;
  res << staticMetaObject.className() << "All options:\n";
  res << lineTemplate.arg("KEYS", 30).arg("CUR", 8).arg("DEF", 8);

  auto ks = keys();
  for (const auto& k : ks) {
    QString currentV = "[NONE]";
    QString defaultV = "[NONE]";
    if (contains(m_optionMap, k))
      currentV = vInfo(m_optionMap.at(k));
    if (contains(m_defaultMap, k))
      defaultV = vInfo(m_defaultMap.at(k));
    res << lineTemplate.arg(k, 30).arg(currentV, 8).arg(defaultV, 8);
  }
  return res.join("");
}

QString ConfigManager::getString(const QString& k) const {
  return QString::fromStdString(get<std::string>(k));
}

QDebug operator<<(QDebug dbg, const ConfigManager& mng) {
  dbg.nospace() << mng.dumpInfo();
  return dbg.maybeSpace();
}

/*
ConfigManager::ConfigManager(const QVariantMap &map)
  : m_data(map), m_defaultValues(map) {}

void ConfigManager::addSettings(const QVariantMap &map)
{
  for (auto &k : map.keys())
    m_data[k] = map[k];
}

void ConfigManager::copySettings(const QVariantMap &map)
{
  for (auto &k : m_data.keys())
    m_data[k] = map[k];
}

void ConfigManager::copySettings(const ConfigManager &mng)
{
  copySettings(mng.m_data);
}

void ConfigManager::addDefaultSettings(const QVariantMap &map)
{
  for (auto &k : map.keys()) {
    m_defaultValues[k] = map[k];
    if (! m_data.keys().contains(k))
      m_data[k] = map[k];
  }
}

void ConfigManager::resetAll()
{
  m_data.clear();
  if (! m_defaultValues.isEmpty())
    addSettings(m_defaultValues);
}

void ConfigManager::reset(const QString &name)
{
  if (m_defaultValues.keys().contains(name))
    m_data[name] = m_defaultValues[name];
}

QVariant& ConfigManager::operator[](const QString &name)
{
  return m_data[name];
}

QVariant ConfigManager::operator[](const QString &name) const
{
  return m_data.value(name);
}

//template<>
//bool ConfigManager::getOption(const QString &name) const
//{
//  return m_data.value(name, false).toBool();
//}

void ConfigManager::printDebugInfo() const
{
#ifndef QT_NO_DEBUG
  qDebug() << QString("[%1]DEBUG
INFO").arg(staticMetaObject.className()); for (const auto &k :
m_data.keys()) { qDebug() << k << m_data[k];
  }
#endif
}
*/
} // namespace paci
