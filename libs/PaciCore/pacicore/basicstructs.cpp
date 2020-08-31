#include "basicstructs.h"

#include "warlock/numbertools.h"

namespace paci {
/*
TimeArea::TimeArea(int s, int d): startTime(s), durationTime(d) {}

int TimeArea::endTime() const
{
  return startTime + durationTime;
}

void TimeArea::setEndTime(int v)
{
  durationTime = warlock::positiveGate(v - startTime);
}

bool TimeArea::isOverlappedWith(TimeArea &other)
{
  return (this->startTime <= other.endTime())
         || (this->endTime() >= other.startTime);
}

bool TimeArea::operator==(const TimeArea &other)
{
  return (this->startTime == other.startTime)
         && (this->durationTime == other.durationTime);
}
*/
Timebase::Timebase(int r)
  : rate(r) {};
Timebase::Timebase(int r, bool df)
  : rate(r)
  , dropframe(df) {
}

QString Timebase::timeString() const {
  return dropframe ? "00:00:00;00" : "00:00:00:00";
}

bool Timebase::operator==(const Timebase& other) {
  return (this->rate == other.rate)
         && (this->dropframe == other.dropframe);
}

Color::Color(double red, double green, double blue)
  : r(red)
  , g(green)
  , b(blue)
  , a(1) {
}

Color::Color(double red, double green, double blue, double alpha)
  : r(red)
  , g(green)
  , b(blue)
  , a(alpha) {
}

bool Color::operator==(const Color& o) {
  return (this->r == o.r) && (this->g == o.g) && (this->b == o.b)
         && (this->a == o.a);
}

Color Color::from255Value(int r, int g, int b, int a) {
  auto gate = warlock::buildRangeGate(0, 255);
  auto f = [=](int i) {
    return double(gate(i)) / 255.0;
  };
  return Color(f(r), f(g), f(b), f(a));
}

QList<int> Color::to255Int() const {
  auto gate = warlock::buildRangeGate(0, 255);
  auto f = [=](double d) {
    return gate(int(255 * d));
  };
  return { f(r), f(g), f(b), f(a) };
}

} // namespace paci
