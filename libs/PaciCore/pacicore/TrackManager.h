#ifndef TRACKMANAGER_H
#define TRACKMANAGER_H

#include "basic_object.h"
#include "pacicore_global.h"
#include "timearea_support.h"
#include "track.h"

#include <QObject>

namespace paci {

class PACICORESHARED_EXPORT TrackManager
  : public BasicObject
  , public TimeareaSupport
  , public QList<Track*> {
  Q_OBJECT
protected:
  /**
   * @brief 最长轨道的缓存，避免频繁计算时长
   */
  TimePoint m_durationCache;

  /**
   * @brief 自动生成并设置一个新的空轨道
   *
   * 但是不会添加进来
   *
   * @return 新轨道的指针
   */
  Track* autoNewTrack();

public:
  explicit TrackManager(QObject* parent = nullptr);

  int clipCount() const;
  Q_PROPERTY(int clipCount READ clipCount)

  Q_INVOKABLE void appendTrack(Track* aTrack);
  Q_INVOKABLE void removeTrackAt(int index);
  Q_INVOKABLE void appendEmptyTrack();
  Q_INVOKABLE void autoInsertClip(Clip* clip);

  TimePoint durationTime() const override;

signals:
  void durationTimeChanged(TimePoint v);

protected slots:
  void forceUpdateDurationCache();
};

} // namespace paci

#endif // TRACKMANAGER_H
