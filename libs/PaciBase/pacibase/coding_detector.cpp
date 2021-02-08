#include "coding_detector.h"

#include <stdio.h>
#include "warlock/hashstring.h"

namespace paci {

CodingDetector::CodingDetector() {
  m_ucd = uchardet_new();
}

CodingDetector::~CodingDetector() {
  uchardet_delete(m_ucd);
}

QString CodingDetector::detect(const char* filename) {
  auto file = fopen(filename, "r");
  m_len = fread(m_buffer, sizeof(char), NUMBER_OF_SAMPLES, file);
  fclose(file);

  if (uchardet_handle_data(m_ucd, m_buffer, m_len) != 0) {
    return QString();
  }

  uchardet_data_end(m_ucd);
  return uchardet_get_charset(m_ucd);
}

QString CodingDetector::detect(const QString& filename) {
  return detect(filename.toStdString().c_str());
}

QStringConverter::Encoding CodingDetector::detect2(const char *filename)
{
  char const* res = detect(filename).toStdString().c_str();
  switch (warlock::hash_switch(res)) {
    case warlock::hash_const("UTF-8"):
      return QStringConverter::Utf8;
    case warlock::hash_const("UTF-16"):
      return QStringConverter::Utf16;
    case warlock::hash_const("UTF-16BE"):
      return QStringConverter::Utf16BE;
    case warlock::hash_const("UTF-16LE"):
      return QStringConverter::Utf16LE;
    case warlock::hash_const("ISO-8859-1"):
      return QStringConverter::Latin1;
    default:
      return QStringConverter::System;
  }
}

} // namespace paci
