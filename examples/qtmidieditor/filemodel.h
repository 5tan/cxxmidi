#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include <cxxmidi/file.hpp>

class FileModel : public QAbstractListModel {
  Q_OBJECT

 public:
  FileModel(QObject *parent = 0);
  virtual ~FileModel();

  virtual int rowCount(const QModelIndex &index) const;
  virtual QVariant data(const QModelIndex &index,
                        int role = Qt::DisplayRole) const;

  void SetFile(cxxmidi::File *file);

  void RemoveTrack(int num);
  void AddTrack(int num);

 private:
  cxxmidi::File *_file;
};

#endif /* FILEMODEL_H */
