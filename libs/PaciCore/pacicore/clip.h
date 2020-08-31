#ifndef PACICLIP_H
#define PACICLIP_H

#include "basic_object.h"
#include "basicstructs.h"
#include "pacicore_global.h"
#include "pacibase/property_macros.h"
#include "timearea_support.h"

#include <QObject>
#include <QString>
#include <QUuid>
#include <string>

namespace paci {
class PACICORESHARED_EXPORT Clip
  : public BasicObject
  , public TimeareaSupport {
  Q_OBJECT
  Q_PROPERTY(std::string uniqueMark READ uniqueMark WRITE setUniqueMark)
  Q_PROPERTY(paci::TimePoint startTime READ startTime WRITE setStartTime
      NOTIFY startTimeChanged)
  Q_PROPERTY(paci::TimePoint durationTime READ durationTime WRITE
      setDurationTime NOTIFY durationTimeChanged)
  Q_PROPERTY(paci::TimePoint endTime READ endTime WRITE setEndTime
      NOTIFY durationTimeChanged)
  Q_PROPERTY(int wordCount READ wordCount NOTIFY contentChanged)

  PA_WRITABLE_PROPERTY(QString, content, Content)

public:
  explicit Clip(const QString& content,
    const TimePoint& start = TimePoint(0),
    const TimePoint& dur = TimePoint(1000),
    QObject* p = nullptr);

  static Clip* fromClip(const Clip* other);

  std::string uniqueMark() const;
  void setUniqueMark(std::string m);

  [[nodiscard]] TimePoint startTime() const override;
  void setStartTime(const TimePoint& value);

  [[nodiscard]] TimePoint durationTime() const override;
  void setDurationTime(const TimePoint& value);

  void setEndTime(const TimePoint& value);

  int wordCount() const;

protected:
  mutable std::string m_uniqueMark { "" };
  TimePoint m_start, m_duration;

signals:
  void startTimeChanged(TimePoint v);
  void durationTimeChanged(TimePoint v);
};

} // namespace paci

#endif // PACICLIP_H
