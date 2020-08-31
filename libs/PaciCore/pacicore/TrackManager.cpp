#include "TrackManager.h"

namespace paci {
TrackManager::TrackManager(QObject* parent)
  : BasicObject(parent)
  , m_durationCache(0) {
}

void TrackManager::forceUpdateDurationCache() {
  TimePoint f { 0 };
  for (auto* t : *this) {
    TimePoint x = t->endTime();
    if (x > f)
      f = x;
  }
  if (f != m_durationCache) {
    m_durationCache = f;
    emit durationTimeChanged(m_durationCache);
  }
}

void TrackManager::appendTrack(Track* aTrack) {
  auto e = aTrack->endTime();
  aTrack->setParent(this);
  this->append(aTrack);
  if (e > m_durationCache) {
    m_durationCache = e;
    emit durationTimeChanged(e);
  };
}

Track* TrackManager::autoNewTrack() {
  int c = this->size() + 1;
  auto* t = new Track(QString::number(c), this);
  return t;
}

void TrackManager::removeTrackAt(int index) {
  auto* t = (*this)[index];
  bool needRefresh = t->endTime() <= m_durationCache;
  this->removeAt(index);
  if (needRefresh)
    forceUpdateDurationCache();
}

int TrackManager::clipCount() const {
  if (this->isEmpty())
    return 0;
  int c = 0;
  for (auto& x : (*this)) {
    c += x->size();
  }
  return c;
}

void TrackManager::appendEmptyTrack() {
  append(autoNewTrack());
}

void TrackManager::autoInsertClip(Clip* clip) {
  if (! isEmpty())
    for (auto* track : *this) {
      int insert_point = track->scanInsertPoint(*clip);
      bool checked = track->checkInsertPoint(insert_point, *clip);
      if (checked) {
        track->forceInsert(insert_point, clip);
        return;
      }
    }
  auto* new_track = autoNewTrack();
  new_track->forceAppend(clip);
  appendTrack(new_track);
}

TimePoint TrackManager::durationTime() const {
  return m_durationCache;
}

} // namespace paci
