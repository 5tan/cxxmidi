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

#include <QStringList>

#include <cxxmidi/guts/utils.hpp>
#include <cxxmidi/instrument.hpp>
#include <cxxmidi/note.hpp>

#include "trackmodel.h"  // NOLINT(build/include_subdir) INCLUDE_NO_DIR

const char* TrackModel::columnNames[] = {"Dt [ticks]", "Data", "Type",
                                         "Description"};

TrackModel::TrackModel(QObject* parent)
    : QAbstractTableModel(parent), track_(0) {}

// cppcheck-suppress unusedFunction LIB_FUNC
Qt::ItemFlags TrackModel::flags(const QModelIndex& index) const {
  if ((index.column() == COLUMN_DT) || (index.column() == COLUMN_DATA))
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

int TrackModel::rowCount(const QModelIndex& /*index*/) const {
  if (track_) return static_cast<int>(track_->size());

  return 0;
}

bool TrackModel::setData(const QModelIndex& index, const QVariant& value,
                         int role) {
  //! @TODO there should be a command history (Undo/Redo)

  if (role == Qt::EditRole) {
    switch (index.column()) {
      case COLUMN_DT: {
        uint32_t val = value.toUInt();
        track_->at(index.row()).SetDt(val);
        return true;
      }
      case COLUMN_DATA: {
        QString in = value.toString();

        if (in.isEmpty()) return false;

        in.remove(']');
        in.remove('[');
        in.remove(' ');
        QStringList list = in.split(',');

        cxxmidi::Event& event = track_->at(index.row());
        event.clear();

        for (int i = 0; i < list.size(); i++) {
          QString elem = list.at(i);
          uint8_t c;
          if (elem.contains("0x")) {
            elem.remove("0x");
            c = elem.toInt(0, 16);
          } else {
            c = elem.toInt();
          }

          event.push_back(c);
        }

        return true;
      }
      default:
        break;
    }
  }
  return false;
}

// cppcheck-suppress unusedFunction LIB_FUNC
int TrackModel::columnCount(const QModelIndex& /*index*/) const {
  return COLUMN_TOTAL;
}

QVariant TrackModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) return QVariant();

  int col = index.column();
  int row = index.row();

  if (role == Qt::DisplayRole) {
    QString r;

    const cxxmidi::Event* event = &track_->at(row);
    switch (col) {
      case COLUMN_DT:
        return QVariant((unsigned int)event->Dt());
      case COLUMN_TYPE:
        return event->GetName().c_str();
      case COLUMN_DESCR: {
        uint8_t type;
        if (event->size())
          type = event->at(0);
        else
          break;

        if (event->ContainsText()) {
          r = QString("\"" + QString(event->GetText().c_str()) + "\" ");
          break;
        }

        if (event->IsMeta(cxxmidi::Event::kTempo)) {
          int tempo = 0;

          if (event->size() >= 5)
            tempo = cxxmidi::utils::ExtractTempo(event->at(2), event->at(3),
                                                 event->at(4));

          r += QString(" tempo=%1us/qn").arg(tempo);

          break;
        }

        if (event->IsMeta()) break;

        r += QString("Ch=%1 ").arg(type & 0x0f);

        if (event->size() <= 1) break;

        switch (type & 0xf0) {
          case cxxmidi::Event::kProgramChange: {
            QString name = cxxmidi::Instrument::GetName(event->at(1)).c_str();
            r += QString("Prog=\"%1\" ").arg(name);
            break;
          }
          case cxxmidi::Event::kChannelAftertouch:
            r += QString("Val=%1 ").arg(event->at(1));
            break;
          default:
            break;
        }

        if (event->size() <= 2) break;

        switch (type & 0xf0) {
          case cxxmidi::Event::kNoteOff:
          case cxxmidi::Event::kNoteOn: {
            QString note = cxxmidi::Note::GetName(event->at(1)).c_str();
            r += QString("Note=%1 Vel=%2 ").arg(note).arg(event->at(2));
            break;
          }
          case cxxmidi::Event::kNoteAftertouch: {
            QString note = cxxmidi::Note::GetName(event->at(1)).c_str();
            r += QString("Note=%1 Pressure=%2 ").arg(note).arg(event->at(2));
            break;
          }
          case cxxmidi::Event::kControlChange: {
            r += QString("Controller=%1 Val=%2 ")
                     .arg(event->at(1))
                     .arg(event->at(2));
            break;
          }
          case cxxmidi::Event::kPitchWheel: {
            uint16_t combined;  // 14 bit
            combined = (uint16_t)event->at(2);
            combined <<= 7;
            combined |= (uint16_t)event->at(1);

            r += QString("Val=%1 ").arg(combined);

            if (combined == 0x2000)
              r += QString("(Center) ");
            else if (combined < 0x2000)
              r += QString("(Down %1) ").arg(0x2000 - combined);
            else
              r += QString("(Up %1) ").arg(combined - 0x2000);

            break;
          }
          default:
            break;
        }

        break;
      }
      case COLUMN_DATA: {
        r += "[";
        for (size_t i = 0; i < event->size(); i++) {
          if (i) r += ", ";
          r += QString("0x%1").arg(event->at(i), 1, 16);
        }
        r += "]";
        break;
      }
      default:
        r = QString("row: %1, col: %2").arg(row).arg(col);
        break;
    }

    return r;
  }

  return QVariant();
}

void TrackModel::SetTrack(cxxmidi::Track* track) {
  track_ = track;
  layoutChanged();
}

void TrackModel::AddEvent(int num) {
  beginInsertRows(QModelIndex(), num, num);
  std::cerr << "num_: " << num << std::endl;
  cxxmidi::Track::iterator it = track_->begin() + num;
  track_->insert(it, cxxmidi::Event());
  endInsertRows();
}

void TrackModel::RemoveEvent(int num) {
  beginRemoveRows(QModelIndex(), num, num);
  cxxmidi::Track::iterator it = track_->begin() + num;
  track_->erase(it);
  endRemoveRows();
}

// cppcheck-suppress unusedFunction LIB_FUNC
QVariant TrackModel::headerData(int section, Qt::Orientation orientation,
                                int role) const {
  if (orientation == Qt::Vertical)
    if (role == Qt::DisplayRole) return QVariant(section);

  // => orientation_ == Qt::Horizontal
  if (role == Qt::DisplayRole) {
    switch (section) {
      case COLUMN_DATA:
      case COLUMN_DT:
      case COLUMN_DESCR:
      case COLUMN_TYPE:
        return QVariant(columnNames[section]);
      default:
        break;
    }
  }
  return QVariant();
}
