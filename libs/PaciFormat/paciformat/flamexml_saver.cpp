#include "flamexml_saver.h"

#include <QFileInfo>
#include <QSaveFile>

namespace paci {

FlameXmlSaver::FlameXmlSaver(
  Sequence* sequence, QUrl saveUrl, QObject* parent)
  : BaseSaver(sequence, saveUrl, parent) {
  m_config.setDefaults({ { "isWellFormatted", true } });
}

void FlameXmlSaver::writeResolutionNode() {
  xml.writeStartElement("resolution");

#define DEFELEM(name)                                                  \
  xml.writeStartElement(#name);                                        \
  xml.writeCharacters("default");                                      \
  xml.writeEndElement();

  DEFELEM(width)
  DEFELEM(height)
  DEFELEM(depth)
  DEFELEM(aspect)
  DEFELEM(scanformat)

#undef DEFELEM

  xml.writeEndElement();
}

QString FlameXmlSaver::pathForTrack(int index) const {
  QFileInfo info(m_saveUrl.toLocalFile());
  auto* track = m_sequence->trackManager()->at(index);
  auto path = info.path();
  auto basename = info.completeBaseName();
  auto suffix = info.suffix();
  auto extraName =
    tr("_轨道%1_%2")
      .arg(index + 1)
      .arg(track->tag().isEmpty() ? tr("无标签") : track->tag());
  return path + "/" + basename + extraName + "." + suffix;
}

void FlameXmlSaver::saveTrack(int index) {
  auto* track = m_sequence->trackManager()->at(index);
  auto track_sequence_name =
    QString("%1-%2").arg(m_sequence->sequenceName()).arg(track->tag());

  QSaveFile out(pathForTrack(index));
  if (! out.open(TEXTWOMODE)) {
    setHasError(true);
    return;
  }

  xml.setDevice(&out);

  xml.writeStartDocument("1.0");
  xml.writeDTD("<!DOCTYPE subtitle>");

  xml.writeStartElement("subtitle");
  xml.writeAttribute("version", "1");

  xml.writeStartElement("name");
  xml.writeCharacters(track_sequence_name);
  xml.writeEndElement();

  xml.writeStartElement("rate");
  xml.writeCharacters(QString::number(m_sequence->timebase().rate));
  xml.writeEndElement();

  writeResolutionNode();

  xml.writeStartElement("timecode");
  xml.writeStartElement("start");
  xml.writeCharacters(
    track->startTime().toTimecode(m_sequence->timebase()));
  xml.writeEndElement();
  xml.writeStartElement("end");
  xml.writeCharacters(
    track->endTime().toTimecode(m_sequence->timebase()));
  xml.writeEndElement();
  xml.writeEndElement(); // timecode

  xml.writeStartElement("video");
  const FontInfo* font =
    track->fontSwitch() ? track->fontInfo() : m_sequence->fontInfo();
  for (const auto* clip : *track) {
    if (m_canceled)
      break;
    if (! isClipSkipped(*clip))
      writeClip(clip, font);
    currentProgressPlusOne();
  }
  xml.writeEndElement(); // video
  xml.writeEndElement(); // subtitle

  if (m_canceled)
    out.cancelWriting();

  out.flush();
  out.commit();
}

void FlameXmlSaver::writeClip(const Clip* clip, const FontInfo* font) {
  xml.writeStartElement("title");

  xml.writeTextElement(
    "start", clip->startTime().toTimecode(m_sequence->timebase()));
  xml.writeTextElement(
    "end", clip->startTime().toTimecode(m_sequence->timebase()));
  xml.writeTextElement("text", clip->content());
  xml.writeTextElement("font", font->family());
  xml.writeTextElement("size", QString::number(font->size()));

  xml.writeStartElement("fontcolor");
  auto color_int = font->color().to255Int();
  xml.writeTextElement("alpha", QString::number(color_int[3]));
  xml.writeTextElement("red", QString::number(color_int[0]));
  xml.writeTextElement("green", QString::number(color_int[1]));
  xml.writeTextElement("blue", QString::number(color_int[2]));
  xml.writeEndElement(); // fontcolor

  xml.writeTextElement("vertical", "0");
  xml.writeTextElement("horizontal", "0");

  xml.writeEndElement(); // title
}

void FlameXmlSaver::doWork() {
  updateTotalProgress(m_sequence->trackManager()->clipCount());
  updateCurrentProgress(0);
  showMessage(tr("每个轨道将会拆分成单独的文件"));
  emit started();

  xml.setAutoFormatting(m_config.get<bool>("isWellFormatted"));

  auto* manager = m_sequence->trackManager();
  for (int index = 0; index < manager->length(); ++index) {
    if (m_canceled)
      break;
    auto* track = manager->at(index);
    if (isTrackSkipped(track))
      continue;
    saveTrack(index);
  }

  emit finished(m_hasError);
}

} // namespace paci
