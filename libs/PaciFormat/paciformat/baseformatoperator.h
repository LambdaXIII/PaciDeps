#ifndef BASEFORMATOPERATOR_H
#define BASEFORMATOPERATOR_H

#include "pacicore/configmanager.h"
#include "pacibase/pacibaseoperator.h"
#include "pacibase/property_macros.h"
#include "pacicore/sequence.h"

#include <QFile>
#include <QIODevice>
#include <QObject>
#include <QThread>

namespace paci {

constexpr QIODevice::OpenMode TEXTWOMODE =
  QIODevice::WriteOnly | QIODevice::Text;

constexpr QIODevice::OpenMode TEXTROMODE =
  QIODevice::ReadOnly | QIODevice::Text;

class BaseFormatOperator: public PaciBaseOperator {
  Q_OBJECT

  //基础属性
  Q_PROPERTY(ConfigManager config READ config)

public:
  explicit BaseFormatOperator(QObject* parent = nullptr);
  //  void bindToThread(QThread &thread);
  ConfigManager& config();

protected:
  void setCurrentProgress(int x);
  void setTotalProgress(int x);
  void addTotalProgress(int x);

  void currentProgressPlusOne();

  QTextCodec* codec();
  virtual bool isTrackSkipped(const Track* track) const;
  [[nodiscard]] virtual bool isClipSkipped(const Clip& clip) const;

  ConfigManager m_config;
};

} // namespace paci

#endif // BASEFORMATOPERATOR_H
