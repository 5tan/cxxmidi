/* *****************************************************************************
Copyright (c) 2018 Stan Chlebicki

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

#ifndef EXAMPLES_QTMIDIEDITOR_TRACKMODEL_H_
#define EXAMPLES_QTMIDIEDITOR_TRACKMODEL_H_

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
  explicit TrackModel(QObject* parent = 0);
  virtual ~TrackModel() = default;

  Qt::ItemFlags flags(const QModelIndex& /*index*/) const override;
  int rowCount(const QModelIndex& index) const override;
  int columnCount(const QModelIndex& index) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex& index, const QVariant& value,
               int role) override;

  void SetTrack(cxxmidi::Track* track);

  void AddEvent(int num);
  void RemoveEvent(int num);

 private:
  cxxmidi::Track* track_;
};

#endif  // EXAMPLES_QTMIDIEDITOR_TRACKMODEL_H_
