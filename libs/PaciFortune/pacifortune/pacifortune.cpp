#include "pacifortune.h"

#include <QFile>
#include <QRandomGenerator>
#include <QTextStream>
#include <stdexcept>

PaciFortune::PaciFortune(QObject* p)
  : QObject(p) {
  QFile o(NORMAL_FORTUNES);
  if (! o.open(QIODevice::ReadOnly))
    throw std::logic_error(tr("默认Fortune文件无法打开").toStdString());
  QTextStream s(&o);
  while (! s.atEnd()) {
    QString line = s.readLine().simplified();
    if (! line.isEmpty())
      m_fortunes << line;
  }
  o.close();
}

const QString PaciFortune::NORMAL_FORTUNES =
  ":/fortune/normal_fortunes";

QString PaciFortune::pickOne() const {
  if (m_fortunes.isEmpty())
    return "";

  int num = QRandomGenerator::global()->bounded(m_fortunes.length());
  return m_fortunes[num];
}

QStringList& PaciFortune::fortunes() {
  return m_fortunes;
}
