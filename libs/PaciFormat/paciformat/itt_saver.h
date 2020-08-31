#ifndef ITT_SAVER_H
#define ITT_SAVER_H

#include "basesaver.h"
#include "paciformat_global.h"

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QMutex>

namespace paci {


class PACIFORMATSHARED_EXPORT ITTSaver
    :public BaseSaver{
  Q_OBJECT
public:
  explicit ITTSaver(
  Sequence* sequence, QUrl saveUrl, QObject* parent = nullptr);

protected:
  const static QString itt_template;
  QString m_treeCache;
  QMutex m_treeCacheLock;
  QString treeString();

  void saveTrack(int index);

public slots:
  void doWork() override;
};


}//ns paci
#endif // ITT_SAVER_H
