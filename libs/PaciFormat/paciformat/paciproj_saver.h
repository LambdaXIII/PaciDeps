#ifndef PACIPROJ_SAVER_H
#define PACIPROJ_SAVER_H

#include "basesaver.h"
#include "paciformat_global.h"

#include <QObject>
#include <QXmlStreamWriter>
//#include "basicmacros.h"
#include "pacicore/basic_object.h"

namespace paci {

class PACIFORMATSHARED_EXPORT PaciProjSaver: public BaseSaver {
  Q_OBJECT
public:
  enum DTDOption { Standalone = 0, SideCar, NoDtd };
  Q_ENUM(DTDOption)

public:
  explicit PaciProjSaver(
    Sequence* sequence, QUrl saveUrl, QObject* parent = nullptr);

  //  DTDOption getConfig(const QString &name);

protected:
  QXmlStreamWriter xml;
  //  void reallyDoWork();
  void writeDTDFile();
  [[nodiscard]] QString docTypeString() const;
  void writeFontInfo(FontInfo* ft);
  void writeTrack(Track* t);
  void writeExtraDatas(const BasicObject& o);

public slots:
  Q_INVOKABLE void doWork() override;
};

} // namespace paci
#endif // PACIPROJ_SAVER_H
