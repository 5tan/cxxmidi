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

#ifndef INCLUDE_CXXMIDI_FILE_HPP_
#define INCLUDE_CXXMIDI_FILE_HPP_

#include <chrono>  // NOLINT() CPP11_INCLUDES
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include <cxxmidi/track.hpp>

namespace cxxmidi {

class File : public std::vector<Track> {
 public:
  inline File();
  inline explicit File(const char *path);

  inline void Load(const char *path);
  inline void SaveAs(const char *path) const;

  inline Track &AddTrack();
  inline File::size_type Tracks() const;

  inline uint16_t TimeDivision() const;
  inline void SetTimeDivision(uint16_t time_division);

  inline std::chrono::microseconds Duration() const;

 private:
  uint16_t time_division_;  // [ticks per quarternote]

  inline void ReadHeaderChunk(std::ifstream &file);
  inline void ReadUnknownChunk(std::ifstream &file);
  inline void ReadTrackChunk(std::ifstream &file);
  inline void ReadEvent(std::ifstream &file, Event *event, bool *track_continue,
                        uint8_t *running_status);

  inline void SaveHeaderChunk(std::ofstream &output_file) const;
  inline void SaveTrackChunk(std::ofstream &output_file,
                             const Track &track) const;
  inline uint32_t SaveEvent(std::ofstream &output_file, const Event &event,
                            uint8_t *last_cmd) const;

  inline static uint32_t Write(std::ofstream &file, const uint8_t *c,
                               std::streamsize size = 1);
};

}  // namespace cxxmidi

#include <cxxmidi/guts/endianness.hpp>
#include <cxxmidi/guts/simulator.hpp>
#include <cxxmidi/guts/utils.hpp>

namespace cxxmidi {

uint32_t File::Write(std::ofstream &file, const uint8_t *c,
                     std::streamsize size) {
  file.write(reinterpret_cast<const char *>(c), size);
  return size;
}

File::File() : time_division_(500) {}

File::File(const char *path) : time_division_(500) { Load(path); }

void File::SaveAs(const char *path) const {
  std::ofstream out_file(path, std::ios_base::binary);

  if (!out_file.good()) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: could not open file " << path << std::endl;
#endif
    return;
  }

  // save header
  SaveHeaderChunk(out_file);

  // loop over tracks
  for (const auto &track : *this) SaveTrackChunk(out_file, track);

  out_file.close();
}

void File::SaveTrackChunk(std::ofstream &output_file,
                          const Track &track) const {
  // save chunk id
  guts::endianness::WriteBe<uint32_t>(output_file, 0x4D54726B);  // "MTrk"

  // write dummy chunk size (we will get back here)
  auto size_pos = output_file.tellp();
  guts::endianness::WriteBe<uint32_t>(output_file, 0);

  uint32_t chunk_size = 0;  // chunk size
  uint8_t last_cmd = 0;

  for (const auto &event : track)
    chunk_size += SaveEvent(output_file, event, &last_cmd);

  // go back to chunk size
  output_file.seekp(size_pos);
  // save chunk size
  guts::endianness::WriteBe<uint32_t>(output_file, chunk_size);

  // go to end of file
  output_file.seekp(0, std::ios_base::end);
}

uint32_t File::SaveEvent(std::ofstream &output_file, const Event &event,
                         uint8_t *last_cmd) const {
  uint32_t r = utils::SaveVlq(output_file, event.Dt());

  int skip_data_bytes;
  if (event.IsSysex()) {
    r += Write(output_file, &event.at(0));  // SysEx type

    uint8_t data_size = static_cast<uint8_t>(event.size()) - 1;
    r += utils::SaveVlq(output_file, data_size);

    skip_data_bytes = 1;
    *last_cmd = 0;
  } else if (event.IsMeta()) {
    r += Write(output_file, event.data(), 2);  // byte 0 and 1

    uint8_t dataSize = static_cast<uint8_t>(event.size()) - 2;
    r += Write(output_file, &dataSize);  // save length

    skip_data_bytes = 2;
    *last_cmd = 0;
  } else {
    const uint8_t nowCmd = event.at(0);
    skip_data_bytes = (*last_cmd == nowCmd);
    *last_cmd = nowCmd;  // save current command if we use running status
  }

  r += Write(output_file, event.data() + skip_data_bytes,
             event.size() - skip_data_bytes);
  return r;
}

void File::SaveHeaderChunk(std::ofstream &output_file) const {
  // save chunk id
  guts::endianness::WriteBe<uint32_t>(output_file, 0x4D546864);  // "MThd"

  // save header size
  guts::endianness::WriteBe<uint32_t>(output_file, 6);

  // save file type
  uint16_t fileType = (Tracks() > 1) ? 1 : 0;
  guts::endianness::WriteBe<uint16_t>(output_file, fileType);

  // save tracks numer
  guts::endianness::WriteBe<uint16_t>(output_file,
                                      static_cast<uint16_t>(Tracks()));

  // save time division
  guts::endianness::WriteBe<uint16_t>(output_file, time_division_);
}

std::chrono::microseconds File::Duration() const {
  guts::Simulator simulator;
  return simulator.Duration(*this);
}

Track &File::AddTrack() {
  push_back(Track());
  return back();
}

File::size_type File::Tracks() const { return size(); }

uint16_t File::TimeDivision() const { return time_division_; }

void File::SetTimeDivision(uint16_t time_division) {
  time_division_ = time_division;
}

void File::Load(const char *path) {
  clear();

  // open file
  std::ifstream file(path, std::ifstream::in | std::ifstream::binary);

  if (!file.is_open()) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: couldn't open: " << path << std::endl;
#endif
    return;
  }

  // calculate file length
  file.seekg(0, std::ifstream::end);
  auto fileLength = file.tellg();
  file.seekg(0, std::ifstream::beg);

  // control counters
  unsigned int i = 0;
  unsigned int headers = 0;

  // loop over chunks while data still in buffer
  while (file.good() && (fileLength - file.tellg())) {
    uint32_t chunk_id = guts::endianness::ReadBe<uint32_t>(file);

    switch (chunk_id) {
      case 0x4D546864:  // "MThd"
        ReadHeaderChunk(file);
        headers++;
        break;

      case 0x4D54726B:  // "MTrk"
        ReadTrackChunk(file);
        break;

      default:
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: ignoring unknown chunk: 0x" << std::hex
                  << static_cast<int>(chunk_id) << std::endl;
#endif
        ReadUnknownChunk(file);
        break;
    }

    if (!i++ && !headers) {
#ifndef CXXMIDI_QUIET
      std::cerr << "CxxMidi: no header chunk "
                   "(probably not a MIDI file)"
                << std::endl;
#endif
      break;
    }
  }
}

void File::ReadHeaderChunk(std::ifstream &file) {
  // read and check header size
  if (guts::endianness::ReadBe<uint32_t>(file) != 6) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: warning: unsupported MIDI file type" << std::endl;
#endif
  }

  // read file type
  uint16_t type = guts::endianness::ReadBe<uint16_t>(file);

#ifndef CXXMIDI_QUIET
  // check file type
  if ((type != 0) && (type != 1))
    std::cerr << "CxxMidi: warning: unsupported MIDI file type: " << type
              << std::endl;
#endif
  // type 0: single track
  // type 1: multi track

  // read tracks number
  uint16_t tracks_num = guts::endianness::ReadBe<uint16_t>(file);

  // reserve vector capacity
  reserve(tracks_num);

  // read time division
  time_division_ = guts::endianness::ReadBe<uint16_t>(file);

#ifndef CXXMIDI_QUIET
  // check time division
  if (time_division_ & 0x8000)
    std::cerr << "CxxMidi: warning: unsupported MIDI file time division"
              << std::endl;
#endif
}

void File::ReadUnknownChunk(std::ifstream &file) {
  // get chunk size
  uint32_t chunk_size = guts::endianness::ReadBe<uint32_t>(file);

  // skip chunk data
  file.seekg(chunk_size, std::ifstream::cur);
}

void File::ReadTrackChunk(std::ifstream &file) {
  // push back new track
  Track &track = AddTrack();

  // read track chunk size
  uint32_t chunk_size = guts::endianness::ReadBe<uint32_t>(file);
  // we will not use this size to read data (we wait for end event)

  uint8_t running_status = 0;  // start with no running status
  auto begin = file.tellg();
  bool track_continue = true;

  // read track data
  while (track_continue) {
    Event &event = track.AddEvent();

    uint32_t dt = utils::GetVlq(file);  // get delta time
    event.SetDt(dt);                    // save event delta time

    // read event data
    ReadEvent(file, &event, &track_continue, &running_status);
  }

#ifndef CXXMIDI_QUIET
  if (chunk_size != (file.tellg() - begin))
    std::cerr << "CxxMidi: warning: track data and track chunk size mismatch"
              << std::endl;
#endif
}

void File::ReadEvent(std::ifstream &file, Event *event, bool *track_continue,
                     uint8_t *running_status) {
  uint8_t cmd = guts::endianness::ReadBe<uint8_t>(file);

  // check running status
  bool incomplete = false;
  if (cmd < 0x80) {
    incomplete = true;  // this flag says: do not read to much later
    event->push_back(*running_status);  // command from previous complete event
    event->push_back(cmd);
    cmd = *running_status;  // swap
  } else {
    *running_status = cmd;  // current command for this track
    event->push_back(cmd);
  }

  // control events
  switch (cmd & 0xf0) {
    // two parameter events
    case Message::kNoteOn:
    case Message::kNoteOff:
    case Message::kNoteAftertouch:
    case Message::kControlChange:
    case Message::kPitchWheel: {
      event->push_back(guts::endianness::ReadBe<uint8_t>(file));
      if (!incomplete)
        event->push_back(guts::endianness::ReadBe<uint8_t>(file));
    } break;

    // one parameter events
    case Message::kProgramChange:
    case Message::kChannelAftertouch: {
      if (!incomplete)
        event->push_back(guts::endianness::ReadBe<uint8_t>(file));
    } break;

    case 0xf0:  // META events or SysEx events
    {
      switch (cmd) {
        case Message::kMeta:  // META events
        {
          uint8_t meta_event_type = guts::endianness::ReadBe<uint8_t>(file);
          event->push_back(meta_event_type);

          switch (meta_event_type) {
            case Message::kSequenceNumber:  // size always 2
            case Message::kText:
            case Message::kCopyright:
            case Message::kTrackName:
            case Message::kInstrumentName:
            case Message::kLyrics:
            case Message::kMarker:
            case Message::kCuePoint:
            case Message::kChannelPrefix:  // size always 1
            case Message::kOutputCable:    // size always 1
            case Message::kEndOfTrack:     // size always 0
            case Message::kTempo:          // size always 3
            case Message::kSmpteOffset:    // size always 5
            case Message::kTimeSignature:
            case Message::kKeySignature: {
              // read string length
              uint8_t strLength = guts::endianness::ReadBe<uint8_t>(file);
              // event_.push_back(strLength);

#ifndef CXXMIDI_QUIET
              if ((meta_event_type == Message::kSequenceNumber) &&
                  (strLength != 2))
                std::cerr << "CxxMidi: sequence number event size is not 2 but "
                          << strLength << std::endl;

              if ((meta_event_type == Message::kChannelPrefix) &&
                  (strLength != 1))
                std::cerr << "CxxMidi: channel prefix event size is not 1 but"
                          << strLength << std::endl;

              if ((meta_event_type == Message::kOutputCable) &&
                  (strLength != 1))
                std::cerr << "CxxMidi: output cable event size is not 1 but"
                          << strLength << std::endl;

              if ((meta_event_type == Message::kTempo) && (strLength != 3))
                std::cerr << "CxxMidi: tempo event size is not 3 but"
                          << strLength << std::endl;

              if ((meta_event_type == Message::kSmpteOffset) &&
                  (strLength != 5))
                std::cerr << "CxxMidi: SMPTE offset event size is not 5 but "
                          << strLength << std::endl;
#endif

              if (meta_event_type == Message::kEndOfTrack) {
#ifndef CXXMIDI_QUIET
                if (strLength != 0)
                  std::cerr << "CxxMidi: end of track event size is not 0 but "
                            << strLength << std::endl;
#endif
                *track_continue = false;
              }

              // read string
              for (int i = 0; i < strLength; i++)
                event->push_back(guts::endianness::ReadBe<uint8_t>(file));
            } break;
            default: {
#ifndef CXXMIDI_QUIET
              std::cerr << "CxxMidi: unknown meta event 0x" << std::hex
                        << meta_event_type << std::endl;
#endif
            } break;
          }  // switch meta_event_type
          break;
        }  // case 0xff, META events
        case Message::kSysExBegin:
        case Message::kSysExEnd: {
          uint32_t size = utils::GetVlq(file);
          for (unsigned int i = 0; i < size; i++)
            event->push_back(guts::endianness::ReadBe<uint8_t>(file));
        } break;
      }  // switch cmd
    }    // case 0xf0
    break;
    default:
#ifndef CXXMIDI_QUIET
      std::cerr << "warning: unknown event " << static_cast<int>(cmd)
                << std::endl;
#endif
      break;
  }
}

}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_FILE_HPP_
