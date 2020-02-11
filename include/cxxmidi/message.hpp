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
    kSequenceNumber = 0x00,  // size 2
    kText = 0x01,
    kCopyright = 0x02,
    kTrackName = 0x03,
    kInstrumentName = 0x04,
    kLyrics = 0x05,
    kMarker = 0x06,
    kCuePoint = 0x07,
    kChannelPrefix = 0x20,  // size 1
    kOutputCable = 0x21,    // size 1
    kEndOfTrack = 0x2f,     // size 0
    kTempo = 0x51,          // size 3
    kSmpteOffset = 0x54,    // size 5
    kTimeSignature = 0x58,
    kKeySignature = 0x59
  };

  Message() = default;
  inline explicit Message(uint8_t b1);
  inline Message(uint8_t b1, uint8_t b2);
  inline Message(uint8_t b1, uint8_t b2, uint8_t b3);

  inline Message::Type GetType() const;
  inline bool IsVoiceCategory() const;
  inline bool IsVoiceCategory(Type type) const;
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

Message::Message(uint8_t b1) { push_back(b1); }

Message::Message(uint8_t b1, uint8_t b2) {
  reserve(2);
  push_back(b1);
  push_back(b2);
}

Message::Message(uint8_t b1, uint8_t b2, uint8_t b3) {
  reserve(3);
  push_back(b1);
  push_back(b2);
  push_back(b3);
}

Message::Type Message::GetType() const {
  if (!empty()) return static_cast<Type>((*this)[0]);
  return kUndefined;
}

bool Message::IsMeta() const {
  if (size() >= 2) return ((*this)[0] == 0xff);
  return false;
}

bool Message::IsMeta(MetaType meta_type) const {
  return IsMeta() && ((*this)[1] == meta_type);
}

bool Message::IsSysex() const {
  if (!empty()) return ((*this)[0] == kSysExBegin) || ((*this)[0] == kSysExEnd);
  return false;
}

bool Message::IsRealtime() const {
  if (!empty()) return (*this)[0] >= 0xf8;
  return false;
}

bool Message::IsVoiceCategory() const {
  if (!empty()) return ((*this)[0] >= 0x80) && ((*this)[0] <= 0xef);
  return false;
}

bool Message::IsVoiceCategory(Type type) const {
  if (!empty()) return ((*this)[0] & 0xf0) == type;
  return false;
}

bool Message::IsSystemCommon() const {
  if (!empty()) return ((*this)[0] >= 0xf0) && ((*this)[0] <= 0xf7);
  return false;
}

bool Message::ContainsText() const {
  if (size() > 1)
    return ((*this)[0] == 0xff) &&
           (((*this)[1] == kText) || ((*this)[1] == kLyrics) ||
            ((*this)[1] == kInstrumentName) || ((*this)[1] == kTrackName) ||
            ((*this)[1] == kCopyright));
  return false;
}

std::string Message::GetText() const {
  std::string r;
  if (ContainsText()) {
    for (size_t i = 2; i < size(); i++) r += static_cast<char>((*this)[i]);
  }
  return r;
}

std::string Message::GetName() const {
  if (IsMeta()) {
    switch ((*this)[1]) {
      case kSequenceNumber:
        return "SequenceNumber";
      case kText:
        return "Text";
      case kCopyright:
        return "Copyright";
      case kTrackName:
        return "TrackName";
      case kInstrumentName:
        return "InstrumentName";
      case kLyrics:
        return "Lyrics";
      case kMarker:
        return "Marker";
      case kCuePoint:
        return "CuePoint";
      case kChannelPrefix:
        return "ChannelPrefix";
      case kOutputCable:
        return "OutputCable";
      case kEndOfTrack:
        return "EndOfTrack";
      case kTempo:
        return "Tempo";
      case kSmpteOffset:
        return "SmpteOffset";
      case kTimeSignature:
        return "TimeSignature";
      case kKeySignature:
        return "KeySignature";
      default:
        break;
    }
  }

  if (size()) {
    switch ((*this)[0]) {
      // System common:
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

      // Realtime:
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

    // Voice:
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
      default:
        break;
    }
  }

  return "";
}

}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_MESSAGE_HPP_
