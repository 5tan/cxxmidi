/* *****************************************************************************
Copyright (c) 2018 5tan 

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

#ifndef INCLUDE_CXXMIDI_MESSAGE_HPP_
#define INCLUDE_CXXMIDI_MESSAGE_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace cxxmidi {

class Message : public std::vector<uint8_t> {
 public:
  enum Type {
    kUndefined = 0x00,

    // voice
    kNoteOff = 0x80,
    kNoteOn = 0x90,
    kNoteAftertouch = 0xa0,
    kControlChange = 0xb0,
    kProgramChange = 0xc0,      // size 1
    kChannelAftertouch = 0xd0,  // size 1
    kPitchWheel = 0xe0,

    // system common
    kSysExBegin = 0xf0,
    kMtcQuarterFrame = 0xf1,
    kSongPositionPointer = 0xf2,
    kSongSelect = 0xf3,
    kTuneRequest = 0xf6,
    kSysExEnd = 0xf7,

    // realtime
    kClock = 0xf8,
    kTick = 0xf9,
    kStart = 0xfa,
    kContinue = 0xfb,
    kStop = 0xfc,
    kActiveSense = 0xfe,
    kReset = 0xff,

    // Meta events
    kMeta = 0xff
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

  Message() = default;
  inline explicit Message(uint8_t b1);
  inline Message(uint8_t b1, uint8_t b2);
  inline Message(uint8_t b1, uint8_t b2, uint8_t b3);

  inline Message::Type GetType() const;
  inline bool IsVoiceCategory() const;
  inline bool IsSystemCommon() const;
  inline bool IsSysex() const;
  inline bool IsRealtime() const;
  inline bool IsMeta() const;
  inline bool IsMeta(MetaType meta_type) const;
  inline bool ContainsText() const;

  inline std::string GetText() const;
  inline std::string GetName() const;
};

}  // namespace cxxmidi

namespace cxxmidi {

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
  return kUndefined;
}

bool Message::IsMeta() const {
  if (this->size() >= 2) return ((*this)[0] == 0xff);
  return false;
}

bool Message::IsMeta(MetaType meta_type) const {
  return this->IsMeta() && ((*this)[1] == meta_type);
}

bool Message::IsSysex() const {
  if (!this->empty())
    return ((*this)[0] == kSysExBegin) || ((*this)[0] == kSysExEnd);
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
  if (this->ContainsText()) {
    for (size_t i = 2; i < this->size(); i++)
      r += static_cast<char>((*this)[i]);
  }
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

  if (this->size()) {
    switch ((*this)[0] & 0xf0) {
      case kNoteOff:
        return "NoteOff";
      case kNoteOn:
        return "NoteOn";
      case kNoteAftertouch:
        return "NoteAftertouch";
      case kControlChange:
        return "ControlChange";
      case kProgramChange:
        return "ProgramChange";
      case kChannelAftertouch:
        return "ChannelAftertouch";
      case kPitchWheel:
        return "PitchWheel";
      case kSysExBegin:
        return "SysExBegin";
      case kMtcQuarterFrame:
        return "MtcQuarterFrame";
      case kSongPositionPointer:
        return "SongPositionPointer";
      case kSongSelect:
        return "SongSelect";
      case kTuneRequest:
        return "TuneRequest";
      case kSysExEnd:
        return "SysExEnd";
      case kClock:
        return "Clock";
      case kTick:
        return "Tick";
      case kStart:
        return "Start";
      case kContinue:
        return "Continue";
      case kStop:
        return "Stop";
      case kActiveSense:
        return "ActiveSense";
      case kReset:
        return "Reset";
      default:
        break;
    }
  }

  return "";
}

}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_MESSAGE_HPP_
