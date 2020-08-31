#include "track.h"

#include "warlock/logicpool.h"

namespace paci {
Track::Track(QObject* parent)
  : BasicObject(parent)
  , m_fontInfo(new FontInfo(this)) {
}

Track::Track(QString tag, QObject* parent)
  : BasicObject(parent)
  , m_fontInfo(new FontInfo(this))
  , m_tag(tag) {
}

Track::Track(Track* other, QObject* parent)
  : BasicObject(parent) {
  this->m_fontInfo = other->m_fontInfo;
  this->m_tag = other->m_tag;
  for (auto x : *other) {
    this->forceAppend(x);
  }
}

TimePoint Track::durationTime() const {
  if (this->isEmpty())
    return TimePoint(0);
  return this->last()->endTime();
}

void Track::forceAppend(const Clip* aClip) {
  auto new_clip = Clip::fromClip(aClip);
  new_clip->setParent(this);
  new_clip->setStartTime(this->endTime());
  this->append(new_clip);
  emit durationTimeChanged(durationTime());
}

int Track::scanInsertPoint(const Clip& aClip) const {
  int i = 0;
  while (i <= this->size()) {
    if (i < this->length())
      if ((*this)[i]->startTime() > aClip.startTime())
        break;
    i++;
  }
  return i;
}

bool Track::checkInsertPoint(int index, const Clip& aClip) const {
  auto judge1 = warlock::LogicPool({
    this->size() <= 2,
    index - 1 >= this->size(),
  });
  if (judge1.anyRight())
    return true;

  int index0 = index - 1 > 0 ? index - 1 : 0;
  int index1 = index > this->size() - 1 ? this->size() - 1 : index;

  auto func = [&](int i) {
    return (*this)[i]->isOverlappedWith(aClip);
  };

  auto judge2 = warlock::LogicPool({ func(index0), func(index1) });

  return ! judge2.anyRight();
}

void Track::forceInsert(int index, Clip* aClip) {
  auto new_clip = Clip::fromClip(aClip);
  new_clip->setParent(this);
  this->insert(index, new_clip);
  emit durationTimeChanged(durationTime());
}

paci::Clip* Track::clipAt(int index) const {
  return at(index);
}
} // namespace paci
