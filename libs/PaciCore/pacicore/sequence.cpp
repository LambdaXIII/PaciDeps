#include "sequence.h"

namespace paci {
Sequence::Sequence(const QString &name, QObject *parent)
  : BasicObject(parent)
  , m_sequenceName(name)
  , m_trackManager(new TrackManager(this))
  , m_fontInfo(new FontInfo(this))
{
  QObject::connect(m_trackManager, &TrackManager::durationTimeChanged,
                   this, &Sequence::durationTimeChanged);
}

Sequence::Sequence(const QString &name,
                   const Timebase &tb,
                   FontInfo *ft,
                   TrackManager *tm,
                   const QString &nt,
                   QObject *parent)
  : BasicObject (parent)
  , m_sequenceName(name)
  , m_trackManager(tm)
  , m_fontInfo(ft)
  , m_timebase(tb)
  , m_note(nt)
{
  QObject::connect(m_trackManager, &TrackManager::durationTimeChanged,
                   this, &Sequence::durationTimeChanged);
  m_trackManager->setParent(this);
  m_fontInfo->setParent(this);
}

TimePoint Sequence::durationTime() const
{
  return m_trackManager->durationTime();
}

}//namespace paci
