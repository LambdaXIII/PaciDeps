#ifndef BASESTRUCT_H
#define BASESTRUCT_H
#include "pacibase/basicmacros.h"
#include "pacicore_global.h"

#include <QObject>

namespace paci {
/*
struct PACICORESHARED_EXPORT TimeArea {
  int startTime{0};
  int durationTime{1};

  TimeArea() = default;
  TimeArea(int s, int d);

  int endTime() const;
  void setEndTime(int v);

  Q_INVOKABLE bool isOverlappedWith(TimeArea &other);

  Q_INVOKABLE bool operator==(const TimeArea &other);

  Q_INVOKABLE AUTO_NON_EQ(TimeArea)

  Q_PROPERTY(int startTime MEMBER startTime)
  Q_PROPERTY(int durationTime MEMBER durationTime)
  Q_PROPERTY(int endTime READ endTime WRITE setEndTime)

private:
  Q_GADGET
};
*/

struct PACICORESHARED_EXPORT Timebase {
  Q_PROPERTY(int rate MEMBER rate CONSTANT)
  Q_PROPERTY(bool dropframe MEMBER dropframe CONSTANT)

  int rate { 24 };
  bool dropframe { false };

  Timebase() = default;
  Timebase(int r);
  Timebase(int r, bool df);

  Q_INVOKABLE bool operator==(const Timebase& other);
  Q_INVOKABLE AUTO_NON_EQ(Timebase);
  Q_INVOKABLE QString timeString() const;

private:
  Q_GADGET
};

struct PACICORESHARED_EXPORT Color {
  double r { 1 }, g { 1 }, b { 1 }, a { 1 };
  Color() = default;
  Color(double red, double green, double blue);
  Color(double red, double green, double blue, double alpha);

  Q_INVOKABLE bool operator==(const Color& o);
  Q_INVOKABLE AUTO_NON_EQ(Color);
  static Color from255Value(int r, int g, int b, int a);
  QList<int> to255Int() const;

  Q_PROPERTY(double r MEMBER r CONSTANT)
  Q_PROPERTY(double g MEMBER g CONSTANT)
  Q_PROPERTY(double b MEMBER b CONSTANT)
  Q_PROPERTY(double a MEMBER a CONSTANT)
private:
  Q_GADGET
};
} // namespace paci

// Q_DECLARE_METATYPE(paci::TimeArea)
Q_DECLARE_METATYPE(paci::Timebase)
Q_DECLARE_METATYPE(paci::Color)

#endif // BASESTRUCT_H
