#ifndef PA_FILELISTMODEL_H
#define PA_FILELISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfo>
#include <QHash>
#include <QMutex>
#include <QMutexLocker>
#include <QSet>
#include <QUrl>
#include <QVariant>

class PAFileListModel: public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(QStringList fileList READ fileList WRITE setFileList NOTIFY
      fileListChanged)

protected:
  QStringList m_fileList;
  QMutex m_listMutex;

public:
  explicit PAFileListModel(QObject* parent = nullptr)
    : QAbstractListModel(parent) {}
  virtual ~PAFileListModel() {};

  const QStringList& fileList() const { return m_fileList; }
  void setFileList(const QStringList& list) {
    beginResetModel();
    m_fileList.clear();
    m_fileList.append(list);
    endResetModel();
  }

  enum Roles {
    AbsPathRole = Qt::UserRole + 1,
    FilenameRole,
    SuffixRole,
    CompleteSuffixRole,
    BasenameRole,
    CompleteBasenameRole,
    AbsDirRole,
    ExistsRole,
    AbsURLRole,
    PathRole, //可能是绝对的或者相对的
    IsFileRole,
    IsDirRole,
    PAFileListCustomRole
  };

  virtual QHash<int, QByteArray> roleNames() const override {
    return { { AbsPathRole, "absPath" }, { FilenameRole, "filename" },
      { SuffixRole, "suffix" },
      { CompleteSuffixRole, "completeSuffix" },
      { BasenameRole, "basename" },
      { CompleteBasenameRole, "completeBasename" },
      { AbsDirRole, "absDir" }, { ExistsRole, "exists" },
      { AbsURLRole, "absURL" }, { PathRole, "path" },
      { IsFileRole, "isFile" }, { IsDirRole, "isDir" } };
  }

  int rowCount(
    const QModelIndex& parent = QModelIndex()) const override {
    if (parent.isValid())
      return 0;
    return m_fileList.length();
  }

  virtual QVariant data(const QModelIndex& index,
    int role = Qt::DisplayRole) const override {
    if (! index.isValid())
      return QVariant();

    int row = index.row();
    if (row >= m_fileList.length())
      return QVariant();

    QFileInfo info(m_fileList[row]);
    switch (role) {
    case Qt::DisplayRole:
    case FilenameRole:
      return info.fileName();
    case AbsPathRole:
      return info.absoluteFilePath();
    case SuffixRole:
      return info.suffix();
    case CompleteSuffixRole:
      return info.completeSuffix();
    case BasenameRole:
      return info.baseName();
    case CompleteBasenameRole:
      return info.completeBaseName();
    case AbsDirRole:
      return info.absolutePath();
    case ExistsRole:
      return info.exists();
    case AbsURLRole:
      return QUrl::fromLocalFile(info.absoluteFilePath());
    case PathRole:
      return info.path();
    case IsFileRole:
      return info.isFile();
    case IsDirRole:
      return info.isDir();
    }
    return QVariant();
  } // data

  Q_SLOT void append(const QString& p) {
    QMutexLocker lock(&m_listMutex);
    int n = rowCount();
    beginInsertRows(QModelIndex(), n, n);
    m_fileList.append(p);
    endInsertRows();
  }

  Q_SLOT void append(const QUrl& u) {
    if (u.isLocalFile())
      append(u.toLocalFile());
  }

  Q_SLOT void autoAppend(const QString& p) {
    QMutexLocker lock(&m_listMutex);
    if (m_fileList.contains(p))
      return;
    append(p);
  }

  Q_SLOT void autoAppend(const QUrl& u) {
    if (u.isLocalFile())
      autoAppend(u.toLocalFile());
  }

  Q_SLOT void arrange() {
    QMutexLocker lock(&m_listMutex);
    QSet<QString> set;
    for (const auto& x : m_fileList)
      set.insert(x);
    if (set.count() != m_fileList.count()) {
      beginResetModel();
      m_fileList.clear();
      for (const auto& x : set)
        m_fileList.append(x);
      endResetModel();
    }
  }

  Q_SLOT void removeAt(int index) {
    if (index >= 0 && index < rowCount()) {
      QMutexLocker lock(&m_listMutex);
      beginRemoveRows(QModelIndex(), index, index);
      m_fileList.removeAt(index);
      endRemoveRows();
    }
  }

  Q_SLOT void clear() {
    beginResetModel();
    m_fileList.clear();
    endResetModel();
  }

signals:
  void fileListChanged();
};

#endif // PA_FILELISTMODEL_H
