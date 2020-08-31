#ifndef BASICMACROS_H
#define BASICMACROS_H
//
////! 定义一个为初始化的QProperty，已经自动注册，需要自行定义信号
//#define PA_PROPERTY(type, lower, upper)                                \
//protected:                                                             \
//  type m_##lower;                                                      \
//  Q_PROPERTY(                                                          \
//    type lower READ lower WRITE set##upper NOTIFY lower##Changed)      \
//public:                                                                \
//  inline type lower() const { return m_##lower; }                      \
//  void set##upper(type x) {                                            \
//    if (m_##lower != x) {                                              \
//      m_##lower = x;                                                   \
//      emit lower##Changed(m_##lower);                                  \
//    }                                                                  \
//  }
//
////使用value初始化一个QProperty，已经自动注册，需要自行定义信号
//#define PA_PROPERTY_VALUE(type, lower, upper, value)                   \
//protected:                                                             \
//  type m_##lower = value;                                              \
//  Q_PROPERTY(                                                          \
//    type lower READ lower WRITE set##upper NOTIFY lower##Changed)      \
//public:                                                                \
//  inline type lower() const { return m_##lower; }                      \
//  void set##upper(type x) {                                            \
//    if (m_##lower != x) {                                              \
//      m_##lower = x;                                                   \
//      emit lower##Changed(m_##lower);                                  \
//    }                                                                  \
//  }
//
///*! 定义一个为初始化的QProperty，已经自动注册，需要自行定义信号
// * setter 使用 const &*/
//#define PA_PROPERTY_REF_SETTER(type, lower, upper)                     \
//protected:                                                             \
//  type m_##lower;                                                      \
//  Q_PROPERTY(                                                          \
//    type lower READ lower WRITE set##upper NOTIFY lower##Changed)      \
//public:                                                                \
//  inline type lower() const { return m_##lower; }                      \
//  void set##upper(const type& x) {                                     \
//    if (m_##lower != x) {                                              \
//      m_##lower = x;                                                   \
//      emit lower##Changed(m_##lower);                                  \
//    }                                                                  \
//  }
//
///**使用value初始化一个QProperty，已经自动注册，需要自行定义信号
// * setter 使用 const &*/
//#define PA_PROPERTY_VALUE_REF_SETTER(type, lower, upper, value)        \
//protected:                                                             \
//  type m_##lower = value;                                              \
//  Q_PROPERTY(                                                          \
//    type lower READ lower WRITE set##upper NOTIFY lower##Changed)      \
//public:                                                                \
//  inline type lower() const { return m_##lower; }                      \
//  void set##upper(const type& x) {                                     \
//    if (m_##lower != x) {                                              \
//      m_##lower = x;                                                   \
//      emit lower##Changed(m_##lower);                                  \
//    }                                                                  \
//  }
//
////只提供getter和setter的定义，不包含实现
//#define PA_PROPERTY_DEF(type, lower, upper)                            \
//public:                                                                \
//  type lower() const;                                                  \
//  \ void set##upper(type x);
//
////提供getter和setter的定义以及初始值，不包含方法内容
//#define PA_PROPERTY_DEF_VALUE(type, lower, upper, value)               \
//public:                                                                \
//  type lower() const;                                                  \
//  \ void set##upper(type x);
//
//#define PA_PROPERTY_RO(type, lower)                                    \
//protected:                                                             \
//  type m_##lower;                                                      \
//  Q_PROPERTY(type lower READ lower)                                    \
//public:                                                                \
//  inline type lower() const { return m_##lower; }
//
//#define PA_PROPERTY_RO_NOREGISTER(type, lower)                         \
//protected:                                                             \
//  type m_##lower;                                                      \
//                                                                       \
//public:                                                                \
//  inline type lower() const { return m_##lower; }
//
//#define PA_PROPERTY_RO_VALUE(type, lower, value)                       \
//protected:                                                             \
//  type m_##lower(value);                                               \
//  Q_PROPERTY(type lower READ lower)                                    \
//public:                                                                \
//  inline type lower() const { return m_##lower; }
//
//#define PA_PROPERTY_RO_VALUE_NOREGISTER(type, lower, value)            \
//protected:                                                             \
//  type m_##lower = value;                                              \
//                                                                       \
//public:                                                                \
//  inline type lower() const { return m_##lower; }

//自动补充不等操作符
#define AUTO_NON_EQ(classname)                                         \
  bool operator!=(const classname& other) {                            \
    return ! operator==(other);                                        \
  }

#endif // BASICMACROS_H
