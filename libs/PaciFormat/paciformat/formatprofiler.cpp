#include "formatprofiler.h"

#include "csv_loader.h"
#include "csv_saver.h"
#include "fcp7xml_loader.h"
#include "fcp7xml_saver.h"
#include "flamexml_loader.h"
#include "flamexml_saver.h"
#include "itt_loader.h"
#include "itt_saver.h"
#include "paciproj_loader.h"
#include "paciproj_saver.h"
#include "srt_loader.h"
#include "srt_saver.h"
#include "text_loader.h"
#include "text_saver.h"

namespace paci {

FormatProfiler::FormatProfiler(int r) {
  Q_UNUSED(r)
  m_formatNames = { { PaciProject, tr("啪词工程文件") },
    { PlainText, tr("纯文本文件") }, { FlameXML, tr("Flame字幕文件") },
    { Fcp7XML, tr("FCP7序列") }, { CSV, tr("CSV表格") },
    { ITT, tr("iTT字幕文件") }, { SRT, tr("SRT字幕文件") } };
}

QString FormatProfiler::tr(const char* sourceText) {
  return QObject::tr(sourceText, "formatprofiler");
}

const QMap<FormatProfiler::Format, QString>
  FormatProfiler::m_formatCodes = { { Unknown, "unknown" },
    { PaciProject, "paci" }, { PlainText, "plaintext" },
    { Fcp7XML, "fcp7xml" }, { FlameXML, "flamexml" }, { CSV, "csv" },
    { ITT, "iTT" }, { SRT, "srt" } };

const QMap<FormatProfiler::Format, QString>
  FormatProfiler::m_formatSuffixes = { { PaciProject, "*.paci" },
    { PlainText, "*.txt" }, { Fcp7XML, "*.xml" }, { FlameXML, "*.xml" },
    { CSV, "*.csv" }, { ITT, "*.itt" }, { SRT, "*.srt" } };

const QMap<FormatProfiler::Format, std::function<BaseLoader*(QUrl)>>
  FormatProfiler::m_loaderCreatorMap = {
    { PaciProject, &createLoader<PaciProjLoader> },
    { PlainText, &createLoader<TextLoader> },
    { Fcp7XML, &createLoader<Fcp7XMLLoader> },
    { CSV, &createLoader<CSVLoader> },
    { ITT, &createLoader<ITTLoader> },
    { FlameXML, &createLoader<FlameXMLLoader> },
    { SRT, &createLoader<SRTLoader> }
  };

const QMap<FormatProfiler::Format,
  std::function<BaseSaver*(Sequence*, QUrl)>>
  FormatProfiler::m_saverCreatorMap = { { PaciProject,
                                          &createSaver<PaciProjSaver> },
    { PlainText, &createSaver<TextSaver> },
    { Fcp7XML, &createSaver<Fcp7XMLSaver> },
    { CSV, &createSaver<CSVSaver> },
    { FlameXML, &createSaver<FlameXmlSaver> },
    { ITT, &createSaver<ITTSaver> }, { SRT, &createSaver<SRTSaver> } };

bool FormatProfiler::hasSaver(Format format) const {
  return m_saverCreatorMap.keys().contains(format);
}

bool FormatProfiler::hasLoader(Format format) const {
  return m_loaderCreatorMap.keys().contains(format);
}

QString FormatProfiler::filterOf(FormatProfiler::Format f) const {
  return m_formatNames[f] + " (" + m_formatSuffixes[f] + ")";
}

QStringList FormatProfiler::allFiltersList() const {
  QStringList filters;

  for (auto i = m_formatNames.constBegin();
       i != m_formatNames.constEnd();
       ++i) {
    filters << filterOf(i.key());
  }
  return filters;
}

QStringList FormatProfiler::allLoaderFiltersList() const {
  QStringList filters;
  for (auto i = m_loaderCreatorMap.constBegin();
       i != m_loaderCreatorMap.constEnd();
       ++i)
    filters << filterOf(i.key());
  return filters;
}

QStringList FormatProfiler::allSaverFiltersList() const {
  QStringList filters;
  for (auto i = m_saverCreatorMap.constBegin();
       i != m_saverCreatorMap.constEnd();
       ++i)
    filters << filterOf(i.key());
  return filters;
}

QString FormatProfiler::allFilters() const {
  return allFiltersList().join(";;");
}

QString FormatProfiler::allLoaderFilters() const {
  return allLoaderFiltersList().join(";;");
}

QString FormatProfiler::allSaverFilters() const {
  return allSaverFiltersList().join(";;");
}

FormatProfiler::Format FormatProfiler::formatFromFilter(
  const QString& filterString) const {
  for (auto i = m_formatNames.constBegin();
       i != m_formatNames.constEnd();
       ++i) {
    if (filterString.contains(i.value()))
      return i.key();
  }
  return Unknown;
}

QString FormatProfiler::nameOf(Format f) const {
  return m_formatNames.value(f, tr("未知格式"));
}

QString FormatProfiler::suffixOf(Format f) const {
  return m_formatSuffixes.value(f, "*.*");
}

QString FormatProfiler::codeOf(Format f) const {
  return m_formatCodes.value(f, "unknown");
}

FormatProfiler::Format FormatProfiler::formatFromCode(
  const QString& code) const {
  for (auto x = m_formatCodes.constBegin();
       x != m_formatCodes.constEnd();
       ++x) {
    if (x.value() == code)
      return x.key();
  }
  return Unknown;
}

BaseLoader* FormatProfiler::forgeLoader(
  const QUrl& path, Format format) {
  auto* loader = m_loaderCreatorMap[format](path);
  loader->config().addOptions(masterConfig);
  return loader;
}

BaseLoader* FormatProfiler::forgeExampleSequenceLoader() {
  QUrl examplePath =
    QUrl::fromLocalFile(":/templates/paci_example_sequence.paci");
  auto* loader = new PaciProjLoader(examplePath);
  return loader;
}

BaseLoader* FormatProfiler::forgeLoader(
  const QUrl& path, const QString& filterString) {
  auto f = formatFromFilter(filterString);
  return forgeLoader(path, f);
}

BaseSaver* FormatProfiler::forgeSaver(
  Sequence* sequence, const QUrl& path, const QString& filterString) {
  auto f = formatFromFilter(filterString);
  return forgeSaver(sequence, path, f);
}

BaseSaver* FormatProfiler::forgeSaver(
  Sequence* sequence, const QUrl& path, Format format) {
  auto* saver = m_saverCreatorMap[format](sequence, path);
  saver->config().addOptions(masterConfig);
  return saver;
}

FormatProfiler::Format FormatProfiler::formatFromSuffix(
  const QString& suffix) const {
  QString suf = QString("*.%1").arg(suffix);

  for (auto i = m_formatSuffixes.constBegin();
       i != m_formatSuffixes.constEnd();
       ++i) {
    if (suf == i.value())
      return i.key();
  }
  return FormatProfiler::Unknown;
}

} // namespace paci
