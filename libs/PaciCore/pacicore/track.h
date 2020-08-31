#ifndef TRACK_H
#define TRACK_H
#include "basic_object.h"
#include "clip.h"
#include "fontinfo.h"
#include "pacicore_global.h"
#include "pacibase/property_macros.h"
#include "timearea_support.h"

#include <QList>
#include <QObject>

namespace paci {
class PACICORESHARED_EXPORT Track
  : public BasicObject
  , public TimeareaSupport
  , public QList<Clip*> {
  Q_OBJECT

  Q_PROPERTY(paci::TimePoint startTime READ startTime CONSTANT)
  Q_PROPERTY(paci::TimePoint durationTime READ durationTime NOTIFY
      durationTimeChanged)
  Q_PROPERTY(
    paci::TimePoint endTime READ endTime NOTIFY durationTimeChanged)

  PA_READONLY_PROPERTY(paci::FontInfo*, fontInfo, FontInfo)
  PA_WRITABLE_PROPERTY(QString, tag, Tag)
  PA_WRITABLE_PROPERTY(bool, fontSwitch, FontSwitch)

public:
  explicit Track(QObject* parent = nullptr);
  explicit Track(QString tag, QObject* parent = nullptr);
  explicit Track(Track* other, QObject* parent = nullptr);
  TimePoint durationTime() const override;
  /*!
   * \brief forceAppend 强制附加一个片段到末尾
   *
   * 将 \c aClip 的 \c startTime 更改到当前轨道
   * 最后一个片段的结束时间后，附加到列表末尾。
   *
   * \param aClip 将要附加的片段
   */
  Q_INVOKABLE void forceAppend(const Clip* aClip);

  /**
   * @brief scanInsertPoint 为片段查找插入点
   * @param aClip 希望插入的片段
   * @return 符合时间顺序的插入点
   */
  Q_INVOKABLE int scanInsertPoint(const Clip& aClip) const;

  /**
   * @brief checkInsertPoint
   * 检查插入点是否合法。即前后片段是否重叠。
   * @param aClip 希望插入的片段
   * @param index 需要判断的插入点
   * @return 可以安全插入返回 true，否则 false
   */
  Q_INVOKABLE bool checkInsertPoint(int index, const Clip& aClip) const;

  Q_INVOKABLE void forceInsert(int index, Clip* aClip);

  Q_INVOKABLE paci::Clip* clipAt(int index) const;

signals:
  void durationTimeChanged(TimePoint t);
};
} // namespace paci

#endif // TRACK_H
