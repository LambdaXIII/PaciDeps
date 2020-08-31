#ifndef FLAMEXML_SAVER_H
#define FLAMEXML_SAVER_H

#include "basesaver.h"

#include <QObject>
#include <QXmlStreamWriter>
namespace paci {

class FlameXmlSaver: public BaseSaver {
  Q_OBJECT
public:
  explicit FlameXmlSaver(
    Sequence* sequence, QUrl saveUrl, QObject* parent = nullptr);

protected:
  QXmlStreamWriter xml;
  void writeResolutionNode();
  QString pathForTrack(int index) const;
  void saveTrack(int index);
  void writeClip(const paci::Clip* clip, const paci::FontInfo* font);
public slots:
  Q_INVOKABLE void doWork() override;
};

} // namespace paci
#endif // FLAMEXML_SAVER_H
