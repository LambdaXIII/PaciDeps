#ifndef CODING_DETECTOR_H
#define CODING_DETECTOR_H

#include <QObject>
#include "uchardet.h"
#include <QStringConverter>

namespace paci {



class CodingDetector
{
public:
  CodingDetector();
  ~CodingDetector();

  QString detect(const char* filename);
  QString detect(const QString &filename);
  QStringConverter::Encoding detect2(const char* filename);

protected:
  static constexpr int NUMBER_OF_SAMPLES = 2048;
  char m_buffer[NUMBER_OF_SAMPLES];
  int m_len;
  uchardet_t m_ucd;
};



}//ns paci
#endif // CODING_DETECTOR_H
