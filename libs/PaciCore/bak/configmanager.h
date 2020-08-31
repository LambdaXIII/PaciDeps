#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

//#include <QVariantMap>
#include "pacicore_global.h"
#include "simpleconfigmanager.h"

#include <QDebug>
#include <QObject>
#include <QString>
#include <string>

namespace paci {

class PACICORESHARED_EXPORT ConfigManager
  : public warlock::SimpleConfigManager<QString, int, std::string,
      bool> {
  Q_GADGET
protected:
  [[nodiscard]] QString vInfo(const OptionVariant& v) const;
  [[nodiscard]] std::string keyToString(
    const QString& key) const override;

public:
  QString dumpInfo() const;
  QString getString(const QString& k) const;
};

QDebug operator<<(QDebug dbg, const ConfigManager& mng);

} // namespace paci

Q_DECLARE_METATYPE(paci::ConfigManager)

#endif // CONFIGMANAGER_H
