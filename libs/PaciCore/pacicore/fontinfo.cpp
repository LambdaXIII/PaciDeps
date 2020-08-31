#include "fontinfo.h"
//#include "patoolfunction.h"
#include "warlock/hashstring.h"

#define PACK(x) QVariant::fromValue(x)

namespace paci {

FontInfo::FontInfo(QObject* parent)
  : QObject(parent)
  , m_dataMap(FontInfo::m_defaultData) {
}

const QVariantMap FontInfo::m_defaultData = {
  { "family", PACK(QString("Arial")) }, { "size", PACK(double(18.0)) },
  { "alignment", PACK(Alignment::Center) }, { "bold", PACK(false) },
  { "italic", PACK(false) }, { "color", PACK(Color(1, 1, 1, 1)) },
  { "lead", PACK(double(0)) }, { "track", PACK(double(0)) },
  { "outlineWidth", PACK(double(1)) },
  { "outlineSoft", PACK(double(5)) },
  { "outlineColor", PACK(Color(0, 0, 0, 1)) },
  { "aspect", PACK(double(1)) }, { "opacity", PACK(double(1)) }
};

const std::function<QVariant(QString, QVariant)>
  FontInfo::m_defaultValueChecker =
    [](QString propName, const QVariant& propValue) -> QVariant {

#define SCASE(x) warlock::hash_const(#x)
#define RES(x) result = PACK(x);
  //如果默认列表不包含这个Key，则不做处理。
  if (! m_defaultData.keys().contains(propName))
    return propValue;

  QVariant result;
  switch (warlock::hash_switch(propName.toStdString().c_str())) {
  case SCASE(family):
    RES(propValue.toString().trimmed());
    break;
  case SCASE(size):
  case SCASE(outlineWidth):
  case SCASE(outlineSoft):
    RES(warlock::positiveGate(propValue.toDouble()));
    break;
  default:
    RES(propValue);
  }
  return result;

#undef SCASE
#undef RES
};

QVariant& FontInfo::operator[](const QString& key) {
  return m_dataMap[key];
}

bool FontInfo::operator==(const FontInfo& other) {
  return this->m_dataMap == other.m_dataMap;
}
bool FontInfo::operator!=(const FontInfo& other) {
  return ! this->operator==(other);
}

QVariantMap FontInfo::toVariantMap() const {
  return m_dataMap;
}

int FontInfo::propertyCount() const {
  return m_dataMap.count();
}

void FontInfo::copyPropertiesFrom(FontInfo* other) {
  for (auto const& key : other->m_dataMap.keys()) {
    this->m_dataMap[key] = other->m_dataMap[key];
  }
}

} // namespace paci

#undef PACK
