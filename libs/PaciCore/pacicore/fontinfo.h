#ifndef FONTINFO_H
#define FONTINFO_H

#include "basicstructs.h"
#include "warlock/numbertools.h"
#include "pacicore_global.h"

#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QVariant>
#include <QVariantMap>

/******* 生成默认属性工具的宏 **********/
#define PROP(type, lower, upper)                                       \
private:                                                               \
  Q_PROPERTY(                                                          \
    type lower READ lower WRITE set##upper NOTIFY lower##Changed)      \
public:                                                                \
  type lower() const { return m_dataMap[#lower].value<type>(); }       \
public Q_SLOTS:                                                        \
  void set##upper(type newValue) {                                     \
    auto x = m_defaultValueChecker(#lower, packValue(newValue));       \
    if (x.value<type>() != m_dataMap[#lower].value<type>()) {          \
      m_dataMap[#lower] = x;                                           \
      emit lower##Changed(x.value<type>());                            \
    }                                                                  \
  }                                                                    \
Q_SIGNALS:                                                             \
  void lower##Changed(type current);

//对数值类型使用不同的比较方法
#define PROP_NUMBER(type, lower, upper)                                \
private:                                                               \
  Q_PROPERTY(                                                          \
    type lower READ lower WRITE set##upper NOTIFY lower##Changed)      \
public:                                                                \
  type lower() const { return m_dataMap[#lower].value<type>(); }       \
public Q_SLOTS:                                                        \
  void set##upper(type newValue) {                                     \
    auto x = m_defaultValueChecker(#lower, packValue(newValue));       \
    if (! warlock::isNumberEquals(                                     \
          x.value<type>(), m_dataMap[#lower].value<type>())) {         \
      m_dataMap[#lower] = x;                                           \
      emit lower##Changed(x.value<type>());                            \
    }                                                                  \
  }                                                                    \
Q_SIGNALS:                                                             \
  void lower##Changed(type current);

/******* 对于指针数据特殊处理 **********/
#define PROP_STAR(type, lower, upper)                                  \
private:                                                               \
  Q_PROPERTY(                                                          \
    type lower READ lower WRITE set##upper NOTIFY lower##Changed)      \
public:                                                                \
  Q_INVOKABLE type lower() const {                                     \
    return m_dataMap[#lower].value<type>();                            \
  }                                                                    \
public Q_SLOTS:                                                        \
  void set##upper(type newValue) {                                     \
    auto x = m_defaultValueChecker(#lower, packValue(newValue));       \
    if (*(x.value<type>()) != *(m_dataMap[#lower].value<type>())) {    \
      m_dataMap[#lower].value<type>()->deleteLater();                  \
      m_dataMap[#lower] = x;                                           \
      m_dataMap[#lower].value<type>()->setParent(this);                \
      emit lower##Changed(x.value<type>());                            \
    }                                                                  \
  }                                                                    \
Q_SIGNALS:                                                             \
  void lower##Changed(type current);

namespace paci {

/**
 * @brief 存放字体信息的元素\n
 * 可以像字典一样使用。
 *
 * @class PAFontInfo
 *
 *
 * PAFontInfo 是一个自定义类，
 * 并非继承于 QObject。
 *
 * PAFontInfo 内置了一系列默认属性，具体如下：
 *
 * 属性名|类型|说明
 * ------|------|-----
 * family|QString|字体名称
 * size|double|文字大小
 * alignment|PAAlignment|文字对齐方式
 * bold|bool|是否为粗体
 * italic|bool|是否为斜体
 * lead|double|字符间距
 * track|doule|行间距
 * color|PAColor*|文本颜色
 * aspect|double|字形宽高比
 * opacity|double|文本不透明度
 *
 * 此外还有如下扩展属性，用于空心字：
 *
 * 属性名|类型|说明
 * outlineWidth|double|线框宽度
 * outlineSoft|double|线框柔和度
 * outlineColor|PAColor|线框颜色
 *
 * 以上这两张表的属性都内置了对应的 getter 和 setter 方法。
 *
 * @note PAFontInfo 支持使用 [] 操作符来读写属性；
 * 在这种情况下，可以以字符串的方式来增加属性。
 */
class PACICORESHARED_EXPORT FontInfo: public QObject {
  Q_OBJECT

public:
  enum Alignment { Left = 1, Center = 2, Right = 3 };
  Q_ENUM(Alignment)

protected:
  /// 内部数据存储
  QVariantMap m_dataMap;

  /// 默认属性定义
  static const QVariantMap m_defaultData;

  /**
   * @brief 对于默认提供的属性进行赋值检查的函数
   */
  static const std::function<QVariant(QString, QVariant)>
    m_defaultValueChecker;

public:
  explicit FontInfo(QObject* parent = nullptr);

  Q_INVOKABLE QVariant& operator[](const QString& key);
  Q_INVOKABLE bool operator==(const FontInfo& other);
  Q_INVOKABLE bool operator!=(const FontInfo& other);

  Q_INVOKABLE QVariantMap toVariantMap() const;

  Q_INVOKABLE int propertyCount() const;

  Q_INVOKABLE QVariant value(const QString& key) const {
    return m_dataMap.value(key);
  }

  template <typename T>
  void setValue(const QString& key, const T& value) {
    m_dataMap[key] = QVariant::fromValue<T>(value);
  }

  template <typename T>
  /**
   * @brief 将一个数值打包成 QVariant
   * @param value 任何类型的参数
   * @return 打包好的 QVariant
   */
  static QVariant packValue(T value) {
    return QVariant::fromValue(value);
  }

  /******** 使用宏生成默认属性 ************/
  PROP(QString, family, Family)
  PROP_NUMBER(double, size, Size)
  PROP(paci::FontInfo::Alignment, alignment, Alignment)
  PROP(bool, italic, Italic)
  PROP(bool, bold, Bold)
  PROP(paci::Color, color, Color)
  PROP_NUMBER(double, lead, Lead)
  PROP_NUMBER(double, track, Track)
  PROP_NUMBER(double, outlineWidth, OutlineWidth)
  PROP_NUMBER(double, outlineSoft, OutlineSoft)
  PROP(paci::Color, outlineColor, OutlineColor)
  PROP_NUMBER(double, aspect, Aspect)
  PROP_NUMBER(double, opacity, Opacity)

  // signals:
  //  void familyChanged(QString current);
  //  void sizeChanged(double current);
  //  void alignmentChanged(FontInfo::Alignment current);
  //  void italicChanged(bool current);
  //  void boldChanged(bool current);
  //  void colorChanged(Color current);
  //  void leadChanged(double current);
  //  void trackChanged(double current);
  //  void outlineWidthChanged(double current);
  //  void outlineSoftChanged(double current);
  //  void outlineColorChanged(Color current);
  //  void aspectChanged(double current);
  //  void opacityChanged(double current);

public:
  //  static QSharedPointer<PAFontInfo> makeDefaultFontInfo();
  void copyPropertiesFrom(FontInfo* other);
};

} // namespace paci
// PA_DECLARE_SHARED_PTR(PAFontInfo)

/********** 解除宏定义 ***************/
#undef PROP
#undef PROP_NUMBER

#endif // FONTINFO_H
