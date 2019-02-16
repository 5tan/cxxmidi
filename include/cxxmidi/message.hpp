#ifndef CXXMIDI_MESSAGE_HPP
#define CXXMIDI_MESSAGE_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace cxxmidi {

class Message : public std::vector<uint8_t> {
 public:
  enum Type {
    Undefined = 0x00,

    // voice
    NoteOff = 0x80,
    NoteOn = 0x90,
    NoteAftertouch = 0xa0,
    ControlChange = 0xb0,
    ProgramChange = 0xc0,      // size 1
    ChannelAftertouch = 0xd0,  // size 1
    PitchWheel = 0xe0,

    // system common
    SysExBegin = 0xf0,
    MtcQuarterFrame = 0xf1,
    SongPositionPointer = 0xf2,
    SongSelect = 0xf3,
    TuneRequest = 0xf6,
    SysExEnd = 0xf7,

    // realtime
    Clock = 0xf8,
    Tick = 0xf9,
    Start = 0xfa,
    Continue = 0xfb,
    Stop = 0xfc,
    ActiveSense = 0xfe,
    Reset = 0xff,

    // Meta events
    Meta = 0xff
  };

  enum MetaType {
    SequenceNumber = 0x00,  // size 2
    Text = 0x01,
    Copyright = 0x02,
    TrackName = 0x03,
    InstrumentName = 0x04,
    Lyrics = 0x05,
    Marker = 0x06,
    CuePoint = 0x07,
    ChannelPrefix = 0x20,  // size 1
    OutputCable = 0x21,    // size 1
    EndOfTrack = 0x2f,     // size 0
    Tempo = 0x51,          // size 3
    SmpteOffset = 0x54,    // size 5
    TimeSignature = 0x58,
    KeySignature = 0x59

  };

  inline Message();
  inline Message(uint8_t b1);
  inline Message(uint8_t b1, uint8_t b2);
  inline Message(uint8_t b1, uint8_t b2, uint8_t b3);

  inline Message::Type GetType() const;
  inline bool IsVoiceCategory() const;
  inline bool IsSystemCommon() const;
  inline bool IsSysex() const;
  inline bool IsRealtime() const;
  inline bool IsMeta() const;
  inline bool IsMeta(MetaType metaType_) const;
  inline bool ContainsText() const;

  inline std::string GetText() const;
  inline std::string GetName() const;

 private:
};

}  // namespace CxxMidi

namespace cxxmidi {

Message::Message() {}

Message::Message(uint8_t b1) { this->push_back(b1); }

Message::Message(uint8_t b1, uint8_t b2) {
  this->reserve(2);
  this->push_back(b1);
  this->push_back(b2);
}

Message::Message(uint8_t b1, uint8_t b2, uint8_t b3) {
  this->reserve(3);
  this->push_back(b1);
  this->push_back(b2);
  this->push_back(b3);
}

Message::Type Message::GetType() const {
  if (!this->empty()) return static_cast<Type>((*this)[0]);
  return Undefined;
}

bool Message::IsMeta() const {
  if (this->size() >= 2) return ((*this)[0] == 0xff);
  return false;
}

bool Message::IsMeta(MetaType metaType_) const {
  return this->IsMeta() && ((*this)[1] == metaType_);
}

bool Message::IsSysex() const {
  if (!this->empty())
    return ((*this)[0] == SysExBegin) || ((*this)[0] == SysExEnd);
  return false;
}

bool Message::IsRealtime() const {
  if (!this->empty()) return (*this)[0] >= 0xf8;
  return false;
}

bool Message::IsVoiceCategory() const {
  if (!this->empty()) return ((*this)[0] >= 0x80) && ((*this)[0] <= 0xef);
  return false;
}

bool Message::IsSystemCommon() const {
  if (!this->empty()) return ((*this)[0] >= 0xf0) && ((*this)[0] <= 0xf7);
  return false;
}

bool Message::ContainsText() const {
  if (this->size() > 1)
    return ((*this)[0] == 0xff) &&
           (((*this)[1] == Text) || ((*this)[1] == Lyrics) ||
            ((*this)[1] == InstrumentName) || ((*this)[1] == TrackName) ||
            ((*this)[1] == Copyright));
  return false;
}

std::string Message::GetText() const {
  std::string r;
  if (this->ContainsText())
    for (size_t i = 2; i < this->size(); i++)
      r += static_cast<char>((*this)[i]);
  return r;
}

std::string Message::GetName() const {
  if (this->IsMeta()) {
    switch ((*this)[1]) {
      case SequenceNumber:
        return "SequenceNumber";
      case Text:
        return "Text";
      case Copyright:
        return "Copyright";
      case TrackName:
        return "TrackName";
      case InstrumentName:
        return "InstrumentName";
      case Lyrics:
        return "Lyrics";
      case Marker:
        return "Marker";
      case CuePoint:
        return "CuePoint";
      case ChannelPrefix:
        return "ChannelPrefix";
      case OutputCable:
        return "OutputCable";
      case EndOfTrack:
        return "EndOfTrack";
      case Tempo:
        return "Tempo";
      case SmpteOffset:
        return "SmpteOffset";
      case TimeSignature:
        return "TimeSignature";
      case KeySignature:
        return "KeySignature";
      default:
        break;
    }
  }

  if (this->size()) switch ((*this)[0] & 0xf0) {
      case NoteOff:
        return "NoteOff";
      case NoteOn:
        return "NoteOn";
      case NoteAftertouch:
        return "NoteAftertouch";
      case ControlChange:
        return "ControlChange";
      case ProgramChange:
        return "ProgramChange";
      case ChannelAftertouch:
        return "ChannelAftertouch";
      case PitchWheel:
        return "PitchWheel";
      case SysExBegin:
        return "SysExBegin";
      case MtcQuarterFrame:
        return "MtcQuarterFrame";
      case SongPositionPointer:
        return "SongPositionPointer";
      case SongSelect:
        return "SongSelect";
      case TuneRequest:
        return "TuneRequest";
      case SysExEnd:
        return "SysExEnd";
      case Clock:
        return "Clock";
      case Tick:
        return "Tick";
      case Start:
        return "Start";
      case Continue:
        return "Continue";
      case Stop:
        return "Stop";
      case ActiveSense:
        return "ActiveSense";
      case Reset:
        return "Reset";
      default:
        break;
    }

  return "";
}

}  // namespace CxxMidi

#endif  // CXXMIDI_MESSAGE_HPP
