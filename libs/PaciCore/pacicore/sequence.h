#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "TrackManager.h"
#include "basicstructs.h"
#include "clip.h"
#include "fontinfo.h"
#include "pacibase/property_macros.h"
#include "pacicore_global.h"

#include <QObject>

namespace paci {
class Sequence
  : public BasicObject
  , public TimeareaSupport {
  Q_OBJECT

  PA_WRITABLE_PROPERTY(QString, sequenceName, sequenceName)
  PA_CONSTANT_PROPERTY(TrackManager*, trackManager)
  PA_CONSTANT_PROPERTY(paci::FontInfo*, fontInfo)
  PA_WRITABLE_PROPERTY(paci::Timebase, timebase, Timebase)
  PA_WRITABLE_PROPERTY(QString, note, Note)

  Q_PROPERTY(paci::TimePoint startTime READ startTime CONSTANT)
  Q_PROPERTY(paci::TimePoint durationTime READ durationTime NOTIFY
      durationTimeChanged)
  Q_PROPERTY(
    paci::TimePoint endTime READ endTime NOTIFY durationTimeChanged)

public:
  explicit Sequence(const QString& name, QObject* parent = nullptr);

  explicit Sequence(const QString& name,
    const Timebase& tb,
    FontInfo* ft,
    TrackManager* tm,
    const QString& nt,
    QObject* parent = nullptr);

  TimePoint durationTime() const override;

signals:
  void durationTimeChanged(TimePoint v);
  //  void sequenceNameChanged(QString value);
  //  void timebaseChanged(Timebase value);
  //  void noteChanged(QString value);
  void fullRefreshTriggered();
};
} // namespace paci

#endif // SEQUENCE_H
