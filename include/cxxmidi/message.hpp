#ifndef CXXMIDI_MESSAGE_HPP
#define CXXMIDI_MESSAGE_HPP

#include <vector>
#include <string>
#include <cxxmidi/guts/stdint.hpp>

namespace CxxMidi {

class Message : public std::vector<uint8_t>
{
public:
    
    enum Type
    {
        Undefined           = 0x00,
        
        // voice
        NoteOff             = 0x80,
        NoteOn              = 0x90,
        NoteAftertouch      = 0xa0,
        ControlChange       = 0xb0,
        ProgramChange       = 0xc0, // size 1
        ChannelAftertouch   = 0xd0, // size 1
        PitchWheel          = 0xe0,
        
        // system common
        SysExBegin          = 0xf0,
        MtcQuarterFrame     = 0xf1,
        SongPositionPointer = 0xf2,
        SongSelect          = 0xf3,
        TuneRequest         = 0xf6,
        SysExEnd            = 0xf7,
        
        // realtime
        Clock               = 0xf8,
        Tick                = 0xf9,
        Start               = 0xfa,
        Continue            = 0xfb,
        Stop                = 0xfc,
        ActiveSense         = 0xfe,
        Reset               = 0xff,

        // Meta events
        Meta                = 0xff
    };
    
    enum MetaType
    {
        SequenceNumber      = 0x00, // size 2
        Text                = 0x01,
        Copyright           = 0x02,
        TrackName           = 0x03,
        InstrumentName      = 0x04,
        Lyrics              = 0x05,
        Marker              = 0x06,
        CuePoint            = 0x07,
        ChannelPrefix       = 0x20, // size 1
        OutputCable         = 0x21, // size 1
        EndOfTrack          = 0x2f, // size 0
        Tempo               = 0x51, // size 3
        SmpteOffset         = 0x54, // size 5
        TimeSignature       = 0x58,
        KeySignature        = 0x59
        
    };
    
    inline Message();
    inline Message(uint8_t b1_);
    inline Message(uint8_t b1_,
                   uint8_t b2_);
    inline Message(uint8_t b1_,
                   uint8_t b2_,
                   uint8_t b3_);
    
    inline Message::Type type() const;
    inline bool isVoiceCategory() const;
    inline bool isSystemCommon() const;
    inline bool isSysex() const;
    inline bool isRealtime() const;
    inline bool isMeta() const;
    inline bool isMeta(MetaType metaType_) const;
    inline bool containsText() const;

    inline std::string text() const;
    inline std::string name() const;
    
private:
    
};

} // namespace CxxMidi

namespace CxxMidi {

Message::Message()
{
}

Message::Message(uint8_t b1_)
{
    this->push_back(b1_);
}

Message::Message(uint8_t b1_,
                 uint8_t b2_)
{
    this->reserve(2);
    this->push_back(b1_);
    this->push_back(b2_);
}

Message::Message(uint8_t b1_,
                 uint8_t b2_,
                 uint8_t b3_)
{
    this->reserve(3);
    this->push_back(b1_);
    this->push_back(b2_);
    this->push_back(b3_);
}

Message::Type Message::type() const
{
    if(!this->empty())
        return static_cast<Type>( (*this)[0] );
    return Undefined;
}

bool Message::isMeta() const
{
    if(this->size() >= 2)
        return ((*this)[0] == 0xff);
    return false;
}

bool Message::isMeta(MetaType metaType_) const
{
    return this->isMeta() && ((*this)[1]==metaType_);
}

bool Message::isSysex() const
{
    if(!this->empty())
        return ((*this)[0] == SysExBegin) || ((*this)[0] == SysExEnd);
    return false;
}

bool Message::isRealtime() const
{
    if(!this->empty())
        return (*this)[0] >= 0xf8 ;
    return false;
}

bool Message::isVoiceCategory() const
{
    if(!this->empty())
        return ((*this)[0] >= 0x80) && ( (*this)[0] <= 0xef);
    return false;
}

bool Message::isSystemCommon() const
{
    if(!this->empty())
        return ((*this)[0] >= 0xf0) && ( (*this)[0] <= 0xf7);
    return false;
}

bool Message::containsText() const
{
    if(this->size() > 1)
        return ((*this)[0]==0xff) &&
                ( ( (*this)[1]==Text ) ||
                ( (*this)[1]==Lyrics ) ||
                ( (*this)[1]==InstrumentName ) ||
                ( (*this)[1]==TrackName ) ||
                ( (*this)[1]==Copyright )
                );
    return false;
}

std::string Message::text() const
{
    std::string r;
    if (this->containsText())
        for(size_t i=2; i<this->size(); i++)
            r += static_cast<char>( (*this)[i] );
    return r;
}

std::string Message::name() const
{

    if(this->isMeta())
    {
        switch((*this)[1])
        {
        case SequenceNumber : return "SequenceNumber";
        case Text           : return "Text";
        case Copyright      : return "Copyright";
        case TrackName      : return "TrackName";
        case InstrumentName : return "InstrumentName";
        case Lyrics         : return "Lyrics";
        case Marker         : return "Marker";
        case CuePoint       : return "CuePoint";
        case ChannelPrefix  : return "ChannelPrefix";
        case OutputCable    : return "OutputCable";
        case EndOfTrack     : return "EndOfTrack";
        case Tempo          : return "Tempo";
        case SmpteOffset    : return "SmpteOffset";
        case TimeSignature  : return "TimeSignature";
        case KeySignature   : return "KeySignature";
        default: break;
        }
    }

    if(this->size())
        switch((*this)[0] & 0xf0)
        {
        case NoteOff             : return "NoteOff";
        case NoteOn              : return "NoteOn";
        case NoteAftertouch      : return "NoteAftertouch";
        case ControlChange       : return "ControlChange";
        case ProgramChange       : return "ProgramChange";
        case ChannelAftertouch   : return "ChannelAftertouch";
        case PitchWheel          : return "PitchWheel";
        case SysExBegin          : return "SysExBegin";
        case MtcQuarterFrame     : return "MtcQuarterFrame";
        case SongPositionPointer : return "SongPositionPointer";
        case SongSelect          : return "SongSelect";
        case TuneRequest         : return "TuneRequest";
        case SysExEnd            : return "SysExEnd";
        case Clock               : return "Clock";
        case Tick                : return "Tick";
        case Start               : return "Start";
        case Continue            : return "Continue";
        case Stop                : return "Stop";
        case ActiveSense         : return "ActiveSense";
        case Reset               : return "Reset";
        default: break;
        }

    return "";
}

} // namespace CxxMidi

#endif // CXXMIDI_MESSAGE_HPP
