#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include <cxxmidi/file.hpp>

class FileModel : public QAbstractListModel {
  Q_OBJECT

 public:
  FileModel(QObject *parent_ = 0);
  virtual ~FileModel();

  virtual int rowCount(const QModelIndex &index_) const;
  virtual QVariant data(const QModelIndex &index_,
                        int role_ = Qt::DisplayRole) const;

  void SetFile(cxxmidi::File *file_);

  void RemoveTrack(int num_);
  void AddTrack(int num_);

 private:
  cxxmidi::File *_file;
};

#endif /* FILEMODEL_H */
