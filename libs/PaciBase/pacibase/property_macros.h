#ifndef PROPERTY_MACROS_H
#define PROPERTY_MACROS_H
#include <QObject>

#define __SETTER_BODY__(name)                                          \
  bool ret = false;                                                    \
  if ((ret = (m_##name != name))) {                                    \
    m_##name = name;                                                   \
    emit name##Changed(m_##name);                                      \
  }                                                                    \
  return ret;

#define PA_WRITABLE_PROPERTY(type, name, upper)                        \
protected:                                                             \
  type m_##name;                                                       \
  Q_PROPERTY(                                                          \
    type name READ name WRITE set##upper NOTIFY name##Changed)         \
                                                                       \
public:                                                                \
  type name() const { return m_##name; }                               \
  Q_SLOT bool set##upper(type name) { __SETTER_BODY__(name); }         \
  Q_SIGNAL void name##Changed(type m_##name);

#define PA_READONLY_PROPERTY(type, name, upper)                        \
protected:                                                             \
  type m_##name;                                                       \
  Q_PROPERTY(type name READ name NOTIFY name##Changed);                \
                                                                       \
public:                                                                \
  type name() const { return m_##name; }                               \
  Q_SLOT bool update##upper(type name) { __SETTER_BODY__(name); };     \
  Q_SIGNAL void name##Changed(type m_##name);

#define PA_CONSTANT_PROPERTY(type, name)                               \
protected:                                                             \
  type m_##name;                                                       \
  Q_PROPERTY(type name READ name CONSTANT)                             \
public:                                                                \
  type name() const { return m_##name; }

#define PA_WRITABLE_CSTREF_PROPERTY(type, name, upper)                 \
protected:                                                             \
  type m_##name;                                                       \
  Q_PROPERTY(                                                          \
    type name READ name WRITE set##upper NOTIFY name##Changed)         \
                                                                       \
public:                                                                \
  const type& name() const { return m_##name; }                        \
  Q_SLOT bool set##upper(const type& name) { __SETTER_BODY__(name) };  \
  Q_SIGNAL void name##Changed(const type& m_##name);

#define PA_READONLY_CSTREF_PROPERTY(type, name, upper)                 \
protected:                                                             \
  type m_##name;                                                       \
  Q_PROPERTY(type name READ name NOTIFY name##Changed)                 \
public:                                                                \
  const type& name() const { return m_##name; }                        \
  Q_SLOT bool update##upper(                                           \
    const type& name) { __SETTER_BODY__(name) };                       \
  Q_SIGNAL void name##Changed(const type& m_##name);

#define PA_CONSTANT_CSTREF_PROPERTY(type, name)                        \
protected:                                                             \
  type m_##name;                                                       \
  Q_PROPERTY(type name READ name CONSTANT)                             \
public:                                                                \
  const type& name() const { return m_##name; }

#endif // PROPERTY_MACROS_H
