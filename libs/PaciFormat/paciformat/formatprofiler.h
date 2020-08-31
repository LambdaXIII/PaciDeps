#ifndef FORMATPROFILER_H
#define FORMATPROFILER_H

#include "baseloader.h"
#include "basesaver.h"
#include "pacicore/configmanager.h"
#include "paciformat_global.h"

#include <QMap>
#include <QObject>
#include <QStringList>
#include <functional>

namespace paci {

template <class T>
BaseLoader* createLoader(QUrl p) {
  return new T(p);
}

template <class S>
BaseSaver* createSaver(Sequence* s, QUrl p) {
  return new S(s, p);
}

class PACIFORMATSHARED_EXPORT FormatProfiler {
  Q_GADGET
public:
  enum Format {
    Unknown,
    PaciProject,
    PlainText,
    Fcp7XML,
    FlameXML,
    CSV,
    ITT,
    SRT
  };

  Q_ENUM(Format)

private:
  QMap<Format, QString> m_formatNames;
  const static QMap<Format, QString> m_formatSuffixes;
  const static QMap<Format, QString> m_formatCodes;

  const static QMap<Format, std::function<BaseLoader*(QUrl)>>
    m_loaderCreatorMap;
  const static QMap<Format, std::function<BaseSaver*(Sequence*, QUrl)>>
    m_saverCreatorMap;

protected:
  static QString tr(const char* sourceText);

public:
  explicit FormatProfiler(int r = 0);
  ConfigManager masterConfig;

  Q_INVOKABLE [[nodiscard]] QString filterOf(Format f) const;

  Q_INVOKABLE [[nodiscard]] QStringList allFiltersList() const;
  Q_INVOKABLE [[nodiscard]] QStringList allLoaderFiltersList() const;
  Q_INVOKABLE [[nodiscard]] QStringList allSaverFiltersList() const;

  Q_INVOKABLE [[nodiscard]] QString allFilters() const;
  Q_INVOKABLE [[nodiscard]] QString allLoaderFilters() const;
  Q_INVOKABLE [[nodiscard]] QString allSaverFilters() const;

  Q_INVOKABLE [[nodiscard]] Format formatFromFilter(
    const QString& filterString) const;

  Q_INVOKABLE [[nodiscard]] Format formatFromSuffix(
    const QString& suffix) const;

  Q_INVOKABLE [[nodiscard]] QString nameOf(Format f) const;

  Q_INVOKABLE [[nodiscard]] QString suffixOf(Format f) const;

  Q_INVOKABLE [[nodiscard]] QString codeOf(Format f) const;
  Format formatFromCode(const QString& code) const;

  Q_INVOKABLE [[nodiscard]] BaseLoader* forgeLoader(
    const QUrl& path, const QString& filterString);
  Q_INVOKABLE [[nodiscard]] BaseLoader* forgeLoader(
    const QUrl& path, Format format);
  Q_INVOKABLE static BaseLoader* forgeExampleSequenceLoader();

  Q_INVOKABLE [[nodiscard]] BaseSaver* forgeSaver(
    Sequence* sequence, const QUrl& path, const QString& filterString);
  Q_INVOKABLE [[nodiscard]] BaseSaver* forgeSaver(
    Sequence* sequence, const QUrl& path, Format format);

  Q_INVOKABLE bool hasSaver(Format format) const;
  Q_INVOKABLE bool hasLoader(Format format) const;

  Q_PROPERTY(QStringList allFiltersList READ allFiltersList CONSTANT)
  Q_PROPERTY(
    QStringList allLoaderFiltersList READ allLoaderFiltersList CONSTANT)
  Q_PROPERTY(
    QStringList allSaverFiltersList READ allSaverFiltersList CONSTANT)
};

} // namespace paci

Q_DECLARE_METATYPE(paci::FormatProfiler)
#endif // FORMATPROFILER_H
