/* *****************************************************************************
Copyright (c) 2018 5tan 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
***************************************************************************** */

#include "filemodel.h"  // NOLINT(build/include_subdir) INCLUDE_NO_DIR

FileModel::FileModel(QObject *parent)
    : QAbstractListModel(parent), file_(nullptr) {}

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
