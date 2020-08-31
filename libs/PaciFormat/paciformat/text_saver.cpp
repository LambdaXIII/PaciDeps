#include "text_saver.h"

#include "warlock/logicpool.h"

#include <QDebug>
#include <QSaveFile>
#include <QTextStream>
#include <fstream>

namespace paci {
TextSaver::TextSaver(Sequence* sequence, QUrl saveUrl, QObject* parent)
  : BaseSaver(sequence, saveUrl, parent) {
  m_config.setDefaults({ { "skipDeactivatedTracks", false } });
}

bool TextSaver::isTrackSkipped(const Track* track) const {
  warlock::LogicPool x { (! track->isActivated()),
    m_config.get<bool>("skipDeactivatedTracks") };
  return x.allRight();
}

void TextSaver::doWork() {
  emit started();

  showMessage(tr("开始保存"));

  auto f_name = m_saveUrl.toLocalFile();
  QSaveFile sf(f_name);

  if (sf.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QTextStream stream(&sf);
    //    stream.setCodec(m_config.get<QString>("codecName").toStdString().c_str());
    //    stream.setCodec(codec());

    auto* mng = sequence()->trackManager();
    updateTotalProgress(mng->clipCount());

    for (Track* track : *mng) {
      if ((! canceled()) && (! isTrackSkipped(track))) {
        for (auto* currentClip : *track) {
          if ((! canceled()) && (! isClipSkipped(*currentClip))) {
            auto cont = currentClip->content().simplified();
            stream << cont << "\n";
            showMessage(cont);
            currentProgressPlusOne();
          }
        }
      }
    }
    if (canceled()) {
      sf.cancelWriting();
      showMessage(tr("导出取消"));
    } else {
      stream.flush();
      sf.commit();
      showMessage(tr("导出完毕"));
    }
  } else {
    showMessage(tr("目标文件无法打开。"));
    setHasError(true);
  }
  updateCurrentProgress(totalProgress());
  emit finished(m_hasError);
}

} // namespace paci
