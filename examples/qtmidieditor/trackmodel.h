#ifndef TRACKMODEL_H
#define TRACKMODEL_H

#include <QAbstractTableModel>
#include <QObject>

#include <cxxmidi/track.hpp>

class TrackModel : public QAbstractTableModel {
  Q_OBJECT

  enum {
    COLUMN_DT = 0,
    COLUMN_DATA = 1,
    COLUMN_TYPE = 2,
    COLUMN_DESCR = 3,
    COLUMN_TOTAL = 4
  };

  static const char* columnNames[];

 public:
  TrackModel(QObject* parent = 0);

  virtual ~TrackModel();

  Qt::ItemFlags flags(const QModelIndex& /*index*/) const;
  virtual int rowCount(const QModelIndex& index) const;
  virtual int columnCount(const QModelIndex& index) const;
  virtual QVariant data(const QModelIndex& index,
                        int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation,
                              int role = Qt::DisplayRole) const;

  bool setData(const QModelIndex& index, const QVariant& value, int role);

  void SetTrack(cxxmidi::Track* track);

  void AddEvent(int num);
  void RemoveEvent(int num);

 private:
  cxxmidi::Track* _track;
};

#endif  // TRACKMODEL_H
