#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include <cxxmidi/file.hpp>

class FileModel  : public QAbstractListModel
{
  Q_OBJECT

public:
  FileModel(QObject *parent_=0);
  virtual ~FileModel();

  virtual int rowCount(const QModelIndex & index_) const;
  virtual QVariant data(const QModelIndex & index_,
      int role_ = Qt::DisplayRole) const;

  void setFile(CxxMidi::File * file_);

  void removeTrack(int num_);
  void addTrack(int num_);

private:
  CxxMidi::File * _file;
};

#endif /* FILEMODEL_H */
