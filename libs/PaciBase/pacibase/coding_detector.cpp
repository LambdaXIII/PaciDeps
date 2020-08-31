#include "coding_detector.h"

#include <stdio.h>

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

} // namespace paci
