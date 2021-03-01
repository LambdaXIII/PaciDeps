#include "clip.h"

#include "warlock/string_tools.h"

#include <string>
#include <utility>

namespace paci {
Clip::Clip(const QString& content,
  const paci::TimePoint& start,
  const paci::TimePoint& dur,
  QObject* p)
  : BasicObject(p) {
  this->m_content = content;
  this->m_start = start;
  this->m_duration = dur;
  connect(this,&Clip::startTimeChanged,this,&Clip::anyTimeChanged);
  connect(this,&Clip::durationTimeChanged,this,&Clip::anyTimeChanged);
}

Clip* Clip::fromClip(const Clip* other) {
  return new Clip(
    other->content(), other->startTime(), other->durationTime());
}

std::string Clip::uniqueMark() const {
  if (m_uniqueMark.empty())
    m_uniqueMark = QUuid::createUuid()
                     .toString(QUuid::WithoutBraces)
                     .toLower()
                     .toStdString();
  return m_uniqueMark;
}

void Clip::setUniqueMark(std::string m) {
  m_uniqueMark = m;
}

TimePoint Clip::startTime() const {
  return m_start;
}

TimePoint Clip::durationTime() const {
  return m_duration;
}

void Clip::setStartTime(const paci::TimePoint& value) {
  if (m_start != value) {
    m_start = value;
    emit startTimeChanged(value);
  }
}

void Clip::setDurationTime(const paci::TimePoint& value) {
  if (m_duration != value) {
    m_duration = value;
    emit durationTimeChanged(value);
  }
}

void Clip::setEndTime(const paci::TimePoint& value) {
  TimePoint new_dur = value - m_start;
  if (m_duration != new_dur) {
    m_duration = new_dur;
    emit durationTimeChanged(new_dur);
  }
}

int Clip::wordCount() const {
  if (m_content.isEmpty())
    return 0;
  auto words = m_content.split(" ", Qt::SkipEmptyParts);
  int result = 0;
  for (const auto& wref : words) {
    int cjk_count = 0;
    bool has_nocjkletter = false;
    std::wstring ww = wref.toStdWString();
    for (const auto& c : ww) {
      if (warlock::string_tools::isCJK(c))
        cjk_count++;
      else
        has_nocjkletter = true;
    }
    result += cjk_count;
    if (has_nocjkletter)
      result++;
  }
  return result;
}

} // namespace paci
