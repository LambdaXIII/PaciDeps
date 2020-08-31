//
// Created by 张珺超 on 2019/10/27.
//

#ifndef PACIONE_TIMEAREA_SUPPORT_H
#define PACIONE_TIMEAREA_SUPPORT_H

#include "TimePoint.h"
namespace paci {
class TimeareaSupport {
public:
  [[nodiscard]] virtual TimePoint startTime() const {
    return TimePoint(0);
  };
  [[nodiscard]] virtual TimePoint durationTime() const = 0;
  [[nodiscard]] virtual TimePoint endTime() const {
    return startTime() + durationTime();
  }

  bool isOverlappedWith(const TimeareaSupport& other) const {
    return (this->startTime() < other.endTime())
           && (this->endTime() > other.startTime());
  }
};
} // namespace paci
#endif // PACIONE_TIMEAREA_SUPPORT_H
