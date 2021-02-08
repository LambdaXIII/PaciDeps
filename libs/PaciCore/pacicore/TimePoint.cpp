#include "TimePoint.h"

namespace paci {

TimePoint::TimePoint(const paci::TimePoint& other) {
  this->m_data = other.m_data;
}

TimePoint::TimePoint(paci::TimePoint::TimeNumber n) {
  m_data = TimePoint::valueValidator(n);
}

TimePoint::TimeNumber TimePoint::valueValidator(
  paci::TimePoint::TimeNumber x) {
  if (x > TimePoint::MAX_TIME)
    return x % TimePoint::MAX_TIME;
  if (x < 0)
    return 0;
  return x;
}

TimePoint TimePoint::operator+(TimePoint::TimeNumber x) const {
  return TimePoint(this->m_data + x);
}

TimePoint TimePoint::operator+(const TimePoint& other) const {
  return TimePoint(this->m_data + other.m_data);
}

TimePoint& TimePoint::operator=(TimePoint::TimeNumber x) {
  m_data = TimePoint::valueValidator(x);
  return *this;
}

TimePoint& TimePoint::operator=(const TimePoint &x){
  m_data = x.m_data;
  return *this;
}

TimePoint TimePoint::operator-(TimePoint::TimeNumber x) const {
  if (this->m_data <= x)
    return TimePoint(0);
  return TimePoint(this->m_data - x);
}

TimePoint TimePoint::operator-(const TimePoint& other) const {
  if (this->m_data <= other.m_data)
    return TimePoint(0);
  return TimePoint(this->m_data - other.m_data);
}

bool TimePoint::operator>(const TimePoint& other) const {
  return this->m_data > other.m_data;
}

bool TimePoint::operator>(TimeNumber x) const {
  TimeNumber f = TimePoint::valueValidator(x);
  return this->m_data > f;
}

bool TimePoint::operator<(const TimePoint& other) const {
  return this->m_data < other.m_data;
}

bool TimePoint::operator<(TimeNumber x) const {
  TimeNumber f = TimePoint::valueValidator(x);
  return this->m_data < f;
}

TimePoint TimePoint::operator*(double x) const {
  return TimePoint(std::round(double(m_data) * x));
}

TimePoint TimePoint::operator/(double x) const {
  return TimePoint(std::round(double(m_data) / x));
}

bool TimePoint::operator>=(const TimePoint& other) const {
  return this->m_data >= other.m_data;
}

bool TimePoint::operator<=(const TimePoint& other) const {
  return this->m_data <= other.m_data;
}

TimePoint TimePoint::fromSeconds(double x) {
  return TimePoint(std::round(x * 1000));
}

double TimePoint::toSeconds() const {
  return double(m_data) / 1000.0;
}

TimePoint TimePoint::fromFrames(int frames, const paci::Timebase& tb) {
  double s = double(frames) / double(tb.rate);
  return TimePoint::fromSeconds(s);
}

bool TimePoint::operator==(const paci::TimePoint& other) const {
  return this->m_data == other.m_data;
}

bool TimePoint::operator==(paci::TimePoint::TimeNumber x) const {
  return m_data == valueValidator(x);
}

int TimePoint::toFrames(const paci::Timebase& tb) const {
  if (m_data == 0)
    return 0;
  double s = this->toSeconds();
  int frames = int(std::round(s * tb.rate));
  return frames;
}

TimePoint TimePoint::fromTimecode(
  const QString& tc, const paci::Timebase& tb) {
  auto s = tc;
  s.replace(";", ":");
  QList<int> ns;
  for (auto& x : s.split(":"))
    ns << x.toInt();
  int frames = ns[0] * 60 * 60 * tb.rate + ns[1] * 60 * tb.rate
               + ns[2] * tb.rate + ns[3];
  double seconds = double(frames) / double(tb.rate);
  return TimePoint::fromSeconds(seconds);
}

QString TimePoint::toTimecode(const paci::Timebase& tb) const {
  auto sep = tb.dropframe ? ";" : ":";
  if (m_data == 0)
    return QString("00:00:00") + sep + QString("00");

  int total_frames = int(std::round(this->toSeconds() * tb.rate));
  int ff = total_frames % tb.rate;
  int rest_seconds = total_frames / tb.rate;
  int ss = rest_seconds % 60;
  int rest_minutes = rest_seconds / 60;
  int mm = rest_minutes % 60;
  int rest_hours = rest_minutes / 60;
  int hh = rest_hours % 24;
  return QString("%1:%2:%3%4%5")
    .arg(hh, 2, 10, QLatin1Char('0'))
    .arg(mm, 2, 10, QLatin1Char('0'))
    .arg(ss, 2, 10, QLatin1Char('0'))
    .arg(sep)
    .arg(ff, 2, 10, QLatin1Char('0'));
}

bool TimePoint::operator!=(const TimePoint& other) const {
  return ! this->operator==(other);
}

int TimePoint::toMillseconds() const {
  return m_data;
}

QString TimePoint::toTimestamp() const {
  if (m_data == 0)
    return "00:00:00,000";
  int fff = toMillseconds() % 1000;
  int total_seconds = toMillseconds() / 1000;
  int ss = total_seconds % 60;
  int total_minutes = total_seconds / 60;
  int mm = total_minutes % 60;
  int total_hours = total_minutes / 60;
  int hh = total_hours % 24;
  return QString("%1:%2:%3,%4")
    .arg(hh, 2, 10, QLatin1Char('0'))
    .arg(mm, 2, 10, QLatin1Char('0'))
    .arg(ss, 2, 10, QLatin1Char('0'))
    .arg(fff, 3, 10, QLatin1Char('0'));
}

TimePoint TimePoint::fromTimestamp(const QString& ts) {
  auto timestamp = ts;
  auto nums = timestamp.replace(",", ":").split(":");
  int p1 = nums[0].toInt() * 60 * 60;
  int p2 = nums[1].toInt() * 60;
  int p3 = nums[2].toInt();
  int data = (p1 + p2 + p3) * 1000 + nums[3].toInt();
  return TimePoint(data);
}

QString TimePoint::toTimestamp8Digital() const {
  if (m_data == 0)
    return "00:00:00:00";
  int fff = toMillseconds() % 1000;
  int total_seconds = toMillseconds() / 1000;
  int ss = total_seconds % 60;
  int total_minutes = total_seconds / 60;
  int mm = total_minutes % 60;
  int total_hours = total_minutes / 60;
  int hh = total_hours % 24;
  return QString("%1:%2:%3:%4")
    .arg(hh, 2, 10, QLatin1Char('0'))
    .arg(mm, 2, 10, QLatin1Char('0'))
    .arg(ss, 2, 10, QLatin1Char('0'))
    .arg(fff / 10, 2, 10, QLatin1Char('0'));
}

TimePoint TimePoint::fromTimestamp8Digital(const QString& ts8d) {
  auto timestamp = ts8d;
  auto nums = timestamp.replace(".", ":").split(":");
  int p1 = nums[0].toInt() * 60 * 60;
  int p2 = nums[1].toInt() * 60;
  int p3 = nums[2].toInt();
  int data = (p1 + p2 + p3) * 1000 + nums[3].toInt() * 10;
  return TimePoint(data);
}

} // namespace paci
