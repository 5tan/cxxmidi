#ifndef TRACKMODEL_H
#define TRACKMODEL_H

#include <QObject>
#include <QAbstractTableModel>

#include <cxxmidi/track.hpp>

class TrackModel : public QAbstractTableModel
{
    Q_OBJECT

    enum {
        COLUMN_DT       = 0,
        COLUMN_DATA     = 1,
        COLUMN_TYPE     = 2,
        COLUMN_DESCR    = 3,
        COLUMN_TOTAL    = 4
    };

    static const char * columnNames[];

public:
    TrackModel(QObject *parent_=0);

    virtual ~TrackModel();

    Qt::ItemFlags flags(const QModelIndex & /*index*/) const;
    virtual int rowCount(const QModelIndex & index_) const;
    virtual int columnCount(const QModelIndex & index_) const;
    virtual QVariant data(const QModelIndex & index_,
                          int role_ = Qt::DisplayRole) const;
    virtual QVariant headerData(int section_, Qt::Orientation orientation_,
                                int role_ = Qt::DisplayRole ) const;

    bool setData(const QModelIndex & index_, const QVariant & value_, int role_);

    void setTrack(CxxMidi::Track* track_);

    void addEvent(int num_);
    void removeEvent(int num_);

private:

    CxxMidi::Track* _track;
};

#endif // TRACKMODEL_H
