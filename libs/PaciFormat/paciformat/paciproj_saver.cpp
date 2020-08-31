#include "paciproj_saver.h"

#include "paciproj_common.h"

#include <QDebug>
#include <QFileInfo>
#include <QSaveFile>

namespace paci {

PaciProjSaver::PaciProjSaver(
  Sequence* sequence, QUrl saveUrl, QObject* parent)
  : BaseSaver(sequence, saveUrl, parent) {
  m_config.setDefaults(
    { { "dtdMode", int(NoDtd) }, { "isWellFormatted", true } });
}

QString PaciProjSaver::docTypeString() const {
  QString result;
  switch (m_config.get<int>("dtdMode")) {
  case Standalone:
    result = QString("<!DOCTYPE %1 [\n %2]>")
               .arg(proj::SEQ, proj::dtdString());
    break;
  case SideCar:
    result = QString("<!DOCTYPE %1 SYSTEM \"%2\">")
               .arg(proj::SEQ, proj::PACIDTDFILENAME);
    break;
  default:
    result = QString("<!DOCTYPE %1>").arg(proj::SEQ);
  }
  return result;
}

void PaciProjSaver::writeFontInfo(FontInfo* ft) {
  xml.writeStartElement(proj::FONT);
  auto dataMap = ft->toVariantMap();

  for (const auto& k : dataMap.keys()) {
    if (proj::FONT_KEYS_NUMBER.contains(k)) {
      xml.writeTextElement(k, QString::number(dataMap[k].toDouble()));
    } else if (proj::FONT_KEYS_COLOR.contains(k)) {
      auto c = dataMap[k].value<Color>();
      xml.writeEmptyElement(k);
      xml.writeAttribute(proj::RED, QString("%1").arg(c.r));
      xml.writeAttribute(proj::GREEN, QString("%1").arg(c.g));
      xml.writeAttribute(proj::BLUE, QString("%1").arg(c.b));
      xml.writeAttribute(proj::ALPHA, QString("%1").arg(c.a));
    } else if (proj::FONT_KEYS_BOOL.contains(k)) {
      xml.writeTextElement(k, proj::bool_value_of(dataMap[k].toBool()));
    } else if (k == proj::ALIGNMENT) {
      xml.writeTextElement(k,
        proj::ALIGNMENT_VALUES[dataMap[k]
                                 .value<FontInfo::Alignment>()]);
    } else {
      xml.writeTextElement(k, dataMap[k].toString());
    }
  }
  xml.writeEndElement(); // fontinfo node
}

void PaciProjSaver::doWork() {
  emit started();
  updateTotalProgress(m_sequence->trackManager()->clipCount());
  updateCurrentProgress(0);

  //  xml.setCodec(codec());
  xml.setAutoFormatting(m_config.get<bool>("isWellFormatted"));

  QSaveFile out(m_saveUrl.toLocalFile());
  if (! out.open(TEXTWOMODE)) {
    setHasError(true);
    emit finished(m_hasError);
    return;
  }

  xml.setDevice(&out);

  xml.writeStartDocument(
    "1.0", m_config.get<int>("dtdMode") == Standalone);
  xml.writeDTD(docTypeString());
  xml.writeStartElement(proj::SEQ);

  xml.writeTextElement(proj::SEQNAME, m_sequence->sequenceName());
  xml.writeTextElement(proj::SEQNOTE, m_sequence->note());

  xml.writeEmptyElement(proj::TIMEBASE);
  xml.writeAttribute(
    proj::TB_RATE, QString::number(m_sequence->timebase().rate));
  xml.writeAttribute(
    proj::TB_DF, proj::bool_value_of(m_sequence->timebase().dropframe));

  writeFontInfo(m_sequence->fontInfo());

  xml.writeStartElement(proj::TRACKMANAGER);
  if (m_config.get<bool>("isWellFormatted")) {
    auto mngCommet = tr("本序列共有轨道%1条，片段%2个")
                       .arg(m_sequence->trackManager()->size())
                       .arg(m_sequence->trackManager()->clipCount());
    xml.writeComment(mngCommet);
  }
  for (Track* t : *(m_sequence->trackManager())) {
    if (! isTrackSkipped(t))
      writeTrack(t);
  }
  xml.writeEndElement(); // trackMaager

  writeExtraDatas(*m_sequence);

  xml.writeEndElement(); // sequence
  xml.writeEndDocument();

  out.flush();
  out.commit();

  if (m_config.get<int>("dtdMode") == SideCar)
    writeDTDFile();

  emit finished(m_hasError);
}

void PaciProjSaver::writeTrack(Track* t) {
  xml.writeStartElement(proj::TRACK);
  xml.writeAttribute(proj::TRACK_TAG, t->tag());
  xml.writeAttribute(
    proj::ACTIVATED, proj::bool_value_of(t->isActivated()));
  xml.writeAttribute(
    proj::TRACKFONTSWITCH, proj::bool_value_of(t->fontSwitch()));

  if (m_config.get<bool>("isWellFormatted")) {
    auto comment =
      tr("本轨道包含片段%1个，片段字体%2。")
        .arg(t->count())
        .arg(t->fontSwitch() ? tr("已启用") : tr("未启用"));
    xml.writeComment(comment);
  }

  writeFontInfo(t->fontInfo());
  writeExtraDatas(*t);

  for (auto* clip : *t) {
    if (canceled()) {
      if (m_config.get<bool>("isWellFormatted"))
        xml.writeComment(tr("写入取消，此处信息可能不完整。"));
      break;
    }

    if (! isClipSkipped(*clip)) {
      xml.writeStartElement(proj::CLIP);
      xml.writeAttribute(proj::STARTTIME,
        QString::number(clip->startTime().toMillseconds()));
      xml.writeAttribute(proj::DURATION,
        QString::number(clip->durationTime().toMillseconds()));
      xml.writeAttribute(
        proj::ACTIVATED, proj::bool_value_of(clip->isActivated()));
      xml.writeAttribute(
        proj::UNIMARK, QString::fromStdString(clip->uniqueMark()));
      //      xml.writeCharacters(clip->content());

      xml.writeTextElement(proj::CONTENT, clip->content());

      writeExtraDatas(*clip);

      xml.writeEndElement(); // clip

      showMessage(clip->content());
    }
    currentProgressPlusOne();
  }

  xml.writeEndElement(); // track node
}

void PaciProjSaver::writeExtraDatas(const BasicObject& o) {
  auto edkeys = o.extraPropertyKeys();
  if (! edkeys.isEmpty())
    for (const auto& x : edkeys) {
      xml.writeStartElement(proj::EXTRA_DATA);
      xml.writeAttribute(proj::EXTRA_DATA_NAME, x);
      xml.writeAttribute(proj::EXTRA_DATA_VALUE, o.extraProperty(x));
      xml.writeEndElement();
    }
}

void PaciProjSaver::writeDTDFile() {
  QFileInfo info(m_saveUrl.toLocalFile());
  auto dpath = info.path() + "/" + proj::PACIDTDFILENAME;
  QSaveFile out(dpath);
  if (! out.open(TEXTWOMODE)) {
    setHasError(true);
    return;
  }

  QTextStream dtd(&out);
  dtd << proj::dtdString();

  out.flush();
  out.commit();
}

} // namespace paci
