#include "filemodel.h"

FileModel::FileModel(QObject *parent_) : QAbstractListModel(parent_) {}

FileModel::~FileModel() {}

int FileModel::rowCount(const QModelIndex & /* index_ */) const {
  if (_file) return static_cast<int>(_file->Tracks());

  return 0;
}

void FileModel::SetFile(cxxmidi::File *file_) {
  _file = file_;
  this->layoutChanged();
}

void FileModel::RemoveTrack(int num_) {
  this->beginRemoveRows(QModelIndex(), num_, num_);
  _file->erase(_file->begin() + num_);
  this->endRemoveRows();
}

void FileModel::AddTrack(int num_) {
  this->beginInsertRows(QModelIndex(), num_, num_);
  cxxmidi::File::iterator it = _file->begin() + num_;
  _file->insert(it, cxxmidi::Track());
  this->endInsertRows();
}

QVariant FileModel::data(const QModelIndex &index_, int role_) const {
  if (!index_.isValid()) return QVariant();

  int row = index_.row();

  if (role_ == Qt::DisplayRole) {
    cxxmidi::Track *track = &_file->at(row);
    return QVariant(QString("%1. %2").arg(row).arg(track->GetName().c_str()));
  }

  return QVariant();
}
