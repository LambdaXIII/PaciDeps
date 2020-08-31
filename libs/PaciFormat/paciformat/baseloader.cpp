#include "baseloader.h"
#include <QFileInfo>
#include <QThread>
#include <QStringList>

namespace paci {


BaseLoader::BaseLoader(QUrl fileUrl, QObject *parent)
  : BaseFormatOperator(parent), m_sourceUrl(fileUrl)
{}

//PABaseLoader::PABaseLoader(const QFile &sourceFile, QObject *parent)
//  : PABaseLoader(sourceFile.fileName(), parent)
//{}

QString BaseLoader::parseSequenceName()
{
  QFileInfo info(m_sourceUrl.toLocalFile());
  return info.baseName();
}

QUrl BaseLoader::sourceUrl() const
{
  return m_sourceUrl;
}

QString BaseLoader::parseNote()
{
  return "";
}

Timebase BaseLoader::parseTimebase()
{
  return {};
}

FontInfo* BaseLoader::parseSequenceFontInfo()
{
  return new FontInfo(this);
}

TrackManager* BaseLoader::parseTrackManager()
{
  return new TrackManager(this);
}

QStringList BaseLoader::previewLines()
{
  return QStringList() << "...";
}

Sequence* BaseLoader::doWork()
{
  emit started();
  auto result = new Sequence(parseSequenceName(),
                             parseTimebase(),
                             parseSequenceFontInfo(),
                             parseTrackManager(),
                             parseNote());
  emit resultReady(result, m_hasError);
  return result;
}

} //namespace paci
