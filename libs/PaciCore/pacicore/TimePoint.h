//
// Created by 张珺超 on 2019/10/26.
//

#ifndef PACIONE_TIMEPOINT_H
#define PACIONE_TIMEPOINT_H

#include "basicstructs.h"
#include "warlock/numbertools.h"
#include "pacicore_global.h"
//#include <QJSValue>

#include <cmath>

namespace paci {

class PACICORESHARED_EXPORT TimePoint {
  Q_GADGET
public:
  using TimeNumber = int;

  TimePoint() = default;

  TimePoint(const TimePoint& other);

  explicit TimePoint(TimeNumber n);

  template <typename N>
  explicit TimePoint(N x) {
    auto s = warlock::positiveGate(x);
    m_data = TimeNumber(std::round(s));
  }

//  template<>
//  explicit TimePoint(QJSValue x){
//    auto s = warlock::positiveGate(x.toNumber());
//    m_data = TimeNumber(std::round(s));
//  }

  // OPERATORS
  Q_INVOKABLE TimePoint& operator=(TimeNumber x);
  Q_INVOKABLE TimePoint& operator=(const TimePoint &x);
  Q_INVOKABLE TimePoint operator+(const TimePoint& other) const;
  Q_INVOKABLE TimePoint operator+(TimeNumber x) const;
  Q_INVOKABLE TimePoint operator-(const TimePoint& other) const;
  Q_INVOKABLE TimePoint operator-(TimeNumber x) const;
  Q_INVOKABLE bool operator>(const TimePoint& other) const;
  Q_INVOKABLE bool operator>(TimeNumber x) const;
  Q_INVOKABLE bool operator<(TimeNumber x) const;
  Q_INVOKABLE bool operator<(const TimePoint& other) const;
  Q_INVOKABLE bool operator==(const TimePoint& other) const;
  Q_INVOKABLE bool operator==(TimeNumber x) const;
  Q_INVOKABLE bool operator!=(const TimePoint& other) const;
  Q_INVOKABLE TimePoint operator*(double x) const;
  Q_INVOKABLE TimePoint operator/(double x) const;
  Q_INVOKABLE bool operator>=(const TimePoint& other) const;
  Q_INVOKABLE bool operator<=(const TimePoint& other) const;

  // FACTORIES & Converts

  Q_INVOKABLE static TimePoint fromSeconds(double x);
  Q_INVOKABLE [[nodiscard]] double toSeconds() const;

  Q_INVOKABLE static TimePoint fromFrames(
    int frames, const Timebase& tb = Timebase());
  [[nodiscard]] Q_INVOKABLE int toFrames(
    const Timebase& tb = Timebase()) const;

  Q_INVOKABLE static TimePoint fromTimecode(
    const QString& tc, const Timebase& tb = Timebase());
  [[nodiscard]] Q_INVOKABLE QString toTimecode(
    const Timebase& tb = Timebase()) const;

  Q_INVOKABLE int toMillseconds() const;

  Q_INVOKABLE QString toTimestamp() const;
  Q_INVOKABLE static TimePoint fromTimestamp(const QString& ts);

  Q_INVOKABLE QString toTimestamp8Digital() const;
  Q_INVOKABLE static TimePoint fromTimestamp8Digital(
    const QString& ts8d);

protected:
  static constexpr TimeNumber MAX_TIME = 24 * 60 * 60 * 1000 - 1;
  TimeNumber m_data { 0 };

  static TimeNumber valueValidator(TimeNumber x);
};

} // namespace paci

Q_DECLARE_METATYPE(paci::TimePoint)

#endif // PACIONE_TIMEPOINT_H
