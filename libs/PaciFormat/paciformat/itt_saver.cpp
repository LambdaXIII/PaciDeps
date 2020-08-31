#include "itt_saver.h"

#include <QDomElement>
#include <QFileInfo>
#include <QSaveFile>
#include <stdexcept>

namespace paci {

ITTSaver::ITTSaver(Sequence* sequence, QUrl saveUrl, QObject* parent)
  : BaseSaver(sequence, saveUrl, parent) {
}

QString ITTSaver::treeString() {
  if (m_treeCache.isEmpty()) {
    m_treeCacheLock.lock();
    QFile file(itt_template);
    if (! file.open(TEXTROMODE))
      throw std::invalid_argument(
        tr("无法打开ITT序列模板").toStdString());
    QTextStream in(&file);
    m_treeCache = in.readAll();
    file.close();
    m_treeCacheLock.unlock();
  }
  return m_treeCache;
}

const QString ITTSaver::itt_template = {
  ":/templates/itt_template.xml"
};

void ITTSaver::doWork() {
  updateTotalProgress(m_sequence->trackManager()->clipCount());
  updateCurrentProgress(0);
  emit started();
  showMessage(tr("每个轨道将分别导出"));

  auto* manager = m_sequence->trackManager();
  for (int index = 0; index < manager->length(); ++index) {
    if (m_canceled)
      break;
    auto* track = manager->at(index);
    if (isTrackSkipped(track))
      continue;
    saveTrack(index);
  }

  emit finished(m_hasError || m_canceled);
}

void ITTSaver::saveTrack(int index) {
  auto* track = m_sequence->trackManager()->at(index);
  showMessage(tr("为轨道%1生成数据结构").arg(index + 1));

  QDomDocument doc;
  doc.setContent(treeString());
  QDomElement doc_root = doc.documentElement();
  QDomElement div_elem =
    doc_root.elementsByTagName("div").item(0).toElement();

  for (auto* clip : *track) {
    if (m_canceled)
      break;
    if (! isClipSkipped(*clip)) {
      auto new_elem = doc.createElement("p");
      new_elem.setAttribute(
        "begin", clip->startTime().toTimecode(m_sequence->timebase()));
      new_elem.setAttribute(
        "end", clip->endTime().toTimecode(m_sequence->timebase()));
      new_elem.setAttribute("region", "bottom");
      new_elem.appendChild(doc.createTextNode(clip->content()));
      div_elem.appendChild(new_elem);
    }
    currentProgressPlusOne();
  }

  showMessage(tr("开始写入"));

  QStringList xmls = doc.toString().split("\n");
  addTotalProgress(xmls.length());
  QFileInfo fileInfo(m_saveUrl.toLocalFile());
  QString target_path =
    fileInfo.path() + "/" + fileInfo.completeBaseName()
    + QString("_%1_%2").arg(index + 1).arg(
      track->tag().isEmpty() ? tr("无标签") : track->tag())
    + "." + fileInfo.suffix();
  QSaveFile out(target_path);
  if (! out.open(TEXTWOMODE)) {
    showMessage(tr("目标文件打不开"));
    out.cancelWriting();
    throw std::invalid_argument(tr("目标文件打不开").toStdString());
  }
  QTextStream s(&out);
  for (const auto& line : xmls) {
    if (m_canceled) {
      out.cancelWriting();
      break;
    }
    s << line << "\n";
    currentProgressPlusOne();
  }
  out.flush();
  out.commit();
  showMessage(tr("文件写入完毕"));
}

} // namespace paci
