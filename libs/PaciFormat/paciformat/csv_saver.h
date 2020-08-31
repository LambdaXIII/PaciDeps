#ifndef CSV_SAVER_H
#define CSV_SAVER_H
#include "basesaver.h"
#include "paciformat_global.h"

#include <QObject>

namespace paci {

class PACIFORMATSHARED_EXPORT CSVSaver: public BaseSaver {
  Q_OBJECT
public:
  explicit CSVSaver(
    Sequence* sequence, QUrl saveUrl, QObject* parent = nullptr);

protected:
  QStringList m_headers;

  //  QString pathForTrack(int index);
  //  void saveTrack(Track* track, const QString& path);

  QStringList forge_row(const Clip* c, int track_index = 0) const;
  std::vector<std::string> convert_row(const QStringList& row) const;

public slots:
  Q_INVOKABLE void doWork() override;
};

} // namespace paci
#endif // CSV_SAVER_H
