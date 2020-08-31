#include "baseformatoperator.h"

#include "warlock/numbertools.h"

#include <QTextCodec>

namespace paci {

BaseFormatOperator::BaseFormatOperator(QObject* parent)
  : PaciBaseOperator(parent) {
  m_config.setDefaults(
    { { "skipEmptyClip", false }, { "skipZeroDurationClip", false },
      { "skipDisabledStuff", false }, { "codecName", "utf-8" } });
}

ConfigManager& BaseFormatOperator::config() {
  return m_config;
}

// void BaseFormatOperator::setTotalProgress(int x) {
//  if (x != m_totalProgress) {
//    m_totalProgress = x;
//    emit totalProgressChanged(x);
//  }
//}

// void BaseFormatOperator::setCurrentProgress(int x) {
//  if (x != m_currentProgress) {
//    m_currentProgress = x;
//    emit currentProgressUpdated(x);
//  }
//}

void BaseFormatOperator::addTotalProgress(int x) {
  if (x != 0) {
    m_currentProgress += x;
    emit currentProgressChanged(m_currentProgress);
  }
}

void BaseFormatOperator::currentProgressPlusOne() {
  updateCurrentProgress(currentProgress() + 1);
}

QTextCodec* BaseFormatOperator::codec() {
  QByteArray ba = m_config.get<QString>("codecName").toLatin1();
  return QTextCodec::codecForName(ba.data());
}

bool BaseFormatOperator::isTrackSkipped(const Track* track) const {
  return m_config.get<bool>("skipDisabledStuff")
         && (! track->isActivated());
}

bool BaseFormatOperator::isClipSkipped(const Clip& clip) const {
  if (m_config.get<bool>("skipDisabledStuff") && (! clip.isActivated()))
    return true;

  if (m_config.get<bool>("skipEmptyClip") && clip.content().isEmpty())
    return true;

  return (m_config.get<bool>("skipZeroDurationClip")
          && (clip.durationTime() == 0));
}

} // namespace paci
