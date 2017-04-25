#include "trackmodel.h"

#include <QStringList>

#include <cxxmidi/instrument.hpp>
#include <cxxmidi/note.hpp>
#include <cxxmidi/utils.hpp>

const char * TrackModel::columnNames[] =
{
    "Dt [ticks]",
    "Data",
    "Type",
    "Description"
};

TrackModel::TrackModel(QObject * parent_)
    : QAbstractTableModel(parent_)
    , _track(0)
{
}


TrackModel::~TrackModel() {}


Qt::ItemFlags TrackModel::flags(const QModelIndex & index_) const
{
    if((index_.column() == COLUMN_DT) || (index_.column() == COLUMN_DATA))
        return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled ;

}

int TrackModel::rowCount(const QModelIndex & /*index_*/) const
{
    if(_track)
        return static_cast<int>(_track->size());

    return 0;
}

bool TrackModel::setData(const QModelIndex & index_, const QVariant & value_, int role_)
{
    //! @TODO there should be a command history (Undo/Redo)

    if (role_ == Qt::EditRole)
    {
        switch(index_.column())
        {
        case COLUMN_DT:
        {
            uint32_t value = value_.toUInt();
            _track->at(index_.row()).setDt(value);
            return true;
        }
        case COLUMN_DATA:
        {
            QString in = value_.toString();

            if(in.isEmpty())
                return false;

            in.remove(']');
            in.remove('[');
            in.remove(' ');
            QStringList list = in.split(',');

            CxxMidi::Event &event = _track->at(index_.row());
            event.clear();

            for(int i=0; i<list.size(); i++)
            {
                QString elem = list.at(i);
                uint8_t c;
                if(elem.contains("0x"))
                {
                    elem.remove("0x");
                    c = elem.toInt(0,16);
                }
                else
                    c=elem.toInt();

                event.push_back(c);
            }

            return true;
        }
        default: break;
        }
    }
    return false;
}

int TrackModel::columnCount(const QModelIndex & /*index_*/) const
{
    return COLUMN_TOTAL;
}

QVariant TrackModel::data(const QModelIndex & index_, int role_) const
{
    if(!index_.isValid())
        return QVariant();

    int col = index_.column();
    int row = index_.row();

    if(role_ == Qt::DisplayRole)
    {
        QString r;

        const CxxMidi::Event * event = &_track->at(row);
        switch(col)
        {
        case COLUMN_DT: return QVariant((unsigned int)event->dt());
        case COLUMN_TYPE: return event->name().c_str();
        case COLUMN_DESCR:
        {
            uint8_t type;
            if(event->size())
                type = event->at(0);
            else
                break;

            if(event->containsText())
            {
                r = QString("\"" + QString(event->text().c_str()) + "\" ");
                break;
            }

            if(event->isMeta(CxxMidi::Event::Tempo))
            {
                int tempo=0;

                if(event->size() >=5 )
                    tempo =CxxMidi::Utils::extractTempo(
                                event->at(2),
                                event->at(3),
                                event->at(4));

                r += QString(" tempo=%1us/qn").arg(tempo);

                break;
            }

            if(event->isMeta())
                break;

            r+= QString("Ch=%1 ").arg(type&0x0f);

            if (event->size()<=1)
                break;



            switch(type & 0xf0)
            {
            case CxxMidi::Event::ProgramChange:
            {
                QString name = CxxMidi::Instrument::name(event->at(1)).c_str();
                r+=QString("Prog=\"%1\" ").arg(name);
                break;
            }
            case CxxMidi::Event::ChannelAftertouch:
                r+=QString("Val=%1 ").arg(event->at(1));break;
            default: break;
            }

            if (event->size()<=2)
                break;

            switch(type & 0xf0)
            {
            case CxxMidi::Event::NoteOff:
            case CxxMidi::Event::NoteOn:
            {
                QString note = CxxMidi::Note::name(event->at(1)).c_str();
                r+= QString("Note=%1 Vel=%2 ").arg(note).arg(event->at(2));
                break;
            }
            case CxxMidi::Event::NoteAftertouch:
            {
                QString note = CxxMidi::Note::name(event->at(1)).c_str();
                r+= QString("Note=%1 Pressure=%2 ").arg(note).arg(event->at(2));
                break;
            }
            case CxxMidi::Event::ControlChange:
            {
                r+= QString("Controller=%1 Val=%2 ").arg(event->at(1)).arg(event->at(2));
                break;
            }
            case CxxMidi::Event::PitchWheel:
            {
                uint16_t combined; // 14 bit
                combined = (uint16_t)event->at(2);
                combined <<= 7;
                combined |= (uint16_t)event->at(1);

                r+=QString("Val=%1 ").arg(combined);

                if(combined == 0x2000)
                    r+= QString("(Center) ");
                else if(combined < 0x2000)
                    r+= QString("(Down %1) ").arg(0x2000 - combined);
                else
                    r+= QString("(Up %1) ").arg(combined - 0x2000);


                break;
            }
            default: break;
            }

            break;
        }
        case COLUMN_DATA:
        {
            r+="[";
            for(size_t i=0; i<event->size(); i++)
            {
                if(i)
                    r+=", ";
                r += QString("0x%1").arg(event->at(i),1,16);
            }
            r+="]";
            break;
        }
        default: r=QString("row: %1, col: %2").arg(row).arg(col); break;
        }

        return r;
    }

    return QVariant();
}

void TrackModel::setTrack(CxxMidi::Track* track_)
{
    _track = track_;
    this->layoutChanged();
}

void TrackModel::addEvent(int num_)
{
    this->beginInsertRows(QModelIndex(),num_,num_);
    std::cerr << "num_: " << num_ << std::endl;
    CxxMidi::Track::iterator it = _track->begin() + num_;
    _track->insert(it,CxxMidi::Event());
    this->endInsertRows();
}

void TrackModel::removeEvent(int num_)
{
    this->beginRemoveRows(QModelIndex(),num_,num_);
    CxxMidi::Track::iterator it = _track->begin() + num_;
    _track->erase(it);
    this->endRemoveRows();
}

QVariant TrackModel::headerData(int section_, Qt::Orientation orientation_, int role_) const
{
    if(orientation_ == Qt::Vertical)
        if(role_ == Qt::DisplayRole)
            return QVariant(section_);

    // => orientation_ == Qt::Horizontal
    if(role_ == Qt::DisplayRole)
        switch(section_)
        {
        case COLUMN_DATA:
        case COLUMN_DT:
        case COLUMN_DESCR:
        case COLUMN_TYPE:
            return QVariant(columnNames[section_]);
        default: return QVariant();
        }
    return QVariant();
}
