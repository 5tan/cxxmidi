#include "filemodel.h"

FileModel::FileModel(QObject *parent) : QAbstractListModel(parent) {}

FileModel::~FileModel() {}

int FileModel::rowCount(const QModelIndex & /* index */) const {
  if (file_) return static_cast<int>(file_->Tracks());

  return 0;
}

void FileModel::SetFile(cxxmidi::File *file) {
  file_ = file;
  this->layoutChanged();
}

void FileModel::RemoveTrack(int num) {
  this->beginRemoveRows(QModelIndex(), num, num);
  file_->erase(file_->begin() + num);
  this->endRemoveRows();
}

void FileModel::AddTrack(int num) {
  this->beginInsertRows(QModelIndex(), num, num);
  cxxmidi::File::iterator it = file_->begin() + num;
  file_->insert(it, cxxmidi::Track());
  this->endInsertRows();
}

QVariant FileModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();

  int row = index.row();

  if (role == Qt::DisplayRole) {
    cxxmidi::Track *track = &file_->at(row);
    return QVariant(QString("%1. %2").arg(row).arg(track->GetName().c_str()));
  }

  return QVariant();
}
