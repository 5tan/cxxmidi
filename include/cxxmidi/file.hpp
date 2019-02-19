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

#ifndef INCLUDE_CXXMIDI_FILE_HPP_
#define INCLUDE_CXXMIDI_FILE_HPP_

#include <chrono>
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
  inline size_t Tracks() const;

  inline uint16_t TimeDivision() const;
  inline void SetTimeDivision(uint16_t time_division);

  inline std::chrono::microseconds Duration() const;

 private:
  uint16_t time_division_;  // [ticks per quarternote]

  inline void ReadHeaderChunk(std::fstream &file);
  inline void ReadUnknownChunk(std::fstream &file);
  inline void ReadTrackChunk(std::fstream &file);
  inline void ReadEvent(std::fstream &file, Event *event, bool *track_continue,
                        uint8_t *running_status);

  inline void SaveHeaderChunk(std::ofstream &output_file) const;
  inline void SaveTrackChunk(std::ofstream &output_file, size_t num) const;
  inline size_t SaveEvent(std::ofstream &output_file, const Event &event,
                          uint8_t *last_cmd) const;

  inline static void Putc(std::ofstream &file, uint8_t c);
};

}  // namespace cxxmidi

#include <cxxmidi/guts/endianness.hpp>
#include <cxxmidi/guts/simulator.hpp>

#include <cxxmidi/guts/utils.hpp>

namespace cxxmidi {

void File::Putc(std::ofstream &file, uint8_t c) {
  file.write(reinterpret_cast<char *>(&c), 1);
}

File::File() : time_division_(500) {}

File::File(const char *path) : time_division_(500) { this->Load(path); }

void File::SaveAs(const char *path) const {
  std::ofstream out_file(path, std::ios_base::binary);

  if (!out_file.good()) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: could not open file " << path << std::endl;
#endif
    return;
  }

  // save header
  this->SaveHeaderChunk(out_file);

  // loop over tracks
  for (unsigned int i = 0; i < this->Tracks(); i++)
    this->SaveTrackChunk(out_file, i);

  out_file.close();
}

void File::SaveTrackChunk(std::ofstream &output_file, size_t num) const {
  const Track &track = this->at(num);

  // save chunk id
  guts::endianness::WriteBe<uint32_t>(output_file, 0x4D54726B);  // "MTrk"

  // write dummy chunk size (we will get back here)
  std::streampos size_pos = output_file.tellp();
  guts::endianness::WriteBe<uint32_t>(output_file, 0);

  uint32_t chunk_size = 0;  // chunk size
  uint8_t last_cmd = 0;

  for (size_t i = 0; i < track.size(); i++) {
    const Event &event = track.at(i);
    chunk_size +=
        static_cast<uint32_t>(this->SaveEvent(output_file, event, &last_cmd));
  }

  // go back to chunk size
  output_file.seekp(size_pos);
  // save chunk size
  guts::endianness::WriteBe<uint32_t>(output_file, chunk_size);

  // go to end of file
  output_file.seekp(0, std::ios_base::end);
}

size_t File::SaveEvent(std::ofstream &output_file, const Event &event,
                       uint8_t *last_cmd) const {
  size_t r = 0;

  if (event.IsSysex()) {
    // save delta time variable-length quantity
    r += utils::SaveVlq(output_file, event.Dt());

    File::Putc(output_file, event.at(0));  // SysEx type
    r++;

    uint8_t data_size = static_cast<uint8_t>(event.size()) - 1;
    r += utils::SaveVlq(output_file, data_size);

    //! @TODO check it
    for (size_t i = 1; i < event.size(); i++) {
      File::Putc(output_file, event.at(i));  // save data bytes
      r++;
    }

    *last_cmd = 0;

    return r;
  }

  if (event.IsMeta()) {
    // save delta time variable-length quantity
    r += utils::SaveVlq(output_file, event.Dt());

    File::Putc(output_file, 0xff);         // byte 0 (event_.at(0))
    File::Putc(output_file, event.at(1));  // byte 1, meta event type
    r += 2;

    uint8_t dataSize = static_cast<uint8_t>(event.size()) - 2;
    File::Putc(output_file, dataSize);  // save length
    r++;

    for (size_t i = 2; i < event.size(); i++) {
      File::Putc(output_file, event.at(i));  // save data bytes
      r++;
    }

    *last_cmd = 0;

    return r;
  }

  uint8_t nowCmd = event.at(0);

  r += utils::SaveVlq(output_file, event.Dt());

  if (*last_cmd != nowCmd) {
    File::Putc(output_file, nowCmd);  // byte 0, event type
    r++;
  }

  for (size_t i = 1; i < event.size(); i++) {
    File::Putc(output_file, event.at(i));  // data bytes
    r++;
  }

  // save current command if we use running status
  *last_cmd = nowCmd;

  return r;
}

void File::SaveHeaderChunk(std::ofstream &output_file) const {
  // save chunk id
  guts::endianness::WriteBe<uint32_t>(output_file, 0x4D546864);  // "MThd"

  // save header size
  guts::endianness::WriteBe<uint32_t>(output_file, 6);

  // save file type (dummy, real value saved later)
  uint16_t fileType = (this->Tracks() > 1) ? 1 : 0;
  guts::endianness::WriteBe<uint16_t>(output_file, fileType);

  // save tracks numer (dummy, real value saved later)
  guts::endianness::WriteBe<uint16_t>(output_file,
                                      static_cast<uint16_t>(this->Tracks()));

  // save time division
  guts::endianness::WriteBe<uint16_t>(output_file, time_division_);
}

std::chrono::microseconds File::Duration() const {
  guts::Simulator simulator;
  return simulator.Duration(*this);
}

Track &File::AddTrack() {
  this->push_back(Track());
  return this->back();
}

size_t File::Tracks() const { return this->size(); }

uint16_t File::TimeDivision() const { return time_division_; }

void File::SetTimeDivision(uint16_t time_division) {
  time_division_ = time_division;
}

void File::Load(const char *path) {
  this->clear();

  // open file
  std::fstream file(path, std::fstream::in | std::fstream::binary);

  if (!file.is_open()) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: couldn't open: " << path << std::endl;
#endif
    return;
  }

  // calculate file length
  file.seekg(0, std::fstream::end);
  std::fstream::streampos fileLength = file.tellg();
  file.seekg(0, std::fstream::beg);

  // control counters
  unsigned int i = 0;
  unsigned int headers = 0;

  // loop over chunks while data still in buffer
  while (file.good() && (fileLength - file.tellg())) {
    uint32_t chunk_id = guts::endianness::ReadBe<uint32_t>(file);

    switch (chunk_id) {
      case 0x4D546864:  // "MThd"
        this->ReadHeaderChunk(file);
        headers++;
        break;

      case 0x4D54726B:  // "MTrk"
        this->ReadTrackChunk(file);
        break;

      default:
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: ignoring unknown chunk: 0x" << std::hex
                  << static_cast<int>(chunk_id) << std::endl;
#endif
        this->ReadUnknownChunk(file);
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

void File::ReadHeaderChunk(std::fstream &file) {
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
  this->reserve(tracks_num);

  // read time division
  time_division_ = guts::endianness::ReadBe<uint16_t>(file);

#ifndef CXXMIDI_QUIET
  // check time division
  if (time_division_ & 0x8000)
    std::cerr << "CxxMidi: warning: unsupported MIDI file time division"
              << std::endl;
#endif
}

void File::ReadUnknownChunk(std::fstream &file) {
  // get chunk size
  uint32_t chunk_size = guts::endianness::ReadBe<uint32_t>(file);

  // skip chunk data
  file.seekg(chunk_size, std::fstream::cur);
}

void File::ReadTrackChunk(std::fstream &file) {
  // push back new track
  Track &track = this->AddTrack();

  // read track chunk size
  uint32_t chunk_size = guts::endianness::ReadBe<uint32_t>(file);
  // we will not use this size to read data (we wait for end event)

  uint8_t running_status = 0;  // start with no running status
  std::fstream::streampos begin = file.tellg();
  bool track_continue = true;

  // read track data
  while (track_continue) {
    // create new event
    Event &event = track.AddEvent();

    // get delta time
    uint32_t dt = utils::GetVlq(file);

    // save event delta time
    event.SetDt(dt);

    // read event data
    this->ReadEvent(file, &event, &track_continue, &running_status);
  }

#ifndef CXXMIDI_QUIET
  if (chunk_size != (file.tellg() - begin))
    std::cerr << "CxxMidi: warning: track data and track chunk size mismatch"
              << std::endl;
#endif
}

void File::ReadEvent(std::fstream &file, Event *event, bool *track_continue,
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
    case Event::kNoteOn:
    case Event::kNoteOff:
    case Event::kNoteAftertouch:
    case Event::kControlChange:
    case Event::kPitchWheel: {
      // get parameter 1
      event->push_back(guts::endianness::ReadBe<uint8_t>(file));

      // get parameter 2
      if (!incomplete)
        event->push_back(guts::endianness::ReadBe<uint8_t>(file));
    } break;

      // one parameter events
    case Event::kProgramChange:
    case Event::kChannelAftertouch: {
      if (!incomplete)
        event->push_back(guts::endianness::ReadBe<uint8_t>(file));
    } break;

    case 0xf0:  // META events or SysEx events
    {
      switch (cmd) {
        case 0xff:  // META events
        {
          uint8_t meta_event_type = guts::endianness::ReadBe<uint8_t>(file);
          event->push_back(meta_event_type);

          switch (meta_event_type) {
            default: {
#ifndef CXXMIDI_QUIET
              std::cerr << "CxxMidi: unknown meta event 0x" << std::hex
                        << meta_event_type << std::endl;
#endif
            } break;
            case Event::SequenceNumber:  // size always 2
            case Event::Text:
            case Event::Copyright:
            case Event::TrackName:
            case Event::InstrumentName:
            case Event::Lyrics:
            case Event::Marker:
            case Event::CuePoint:
            case Event::ChannelPrefix:  // size always 1
            case Event::OutputCable:    // size always 1
            case Event::EndOfTrack:     // size always 0
            case Event::Tempo:          // size always 3
            case Event::SmpteOffset:    // size always 5
            case Event::TimeSignature:
            case Event::KeySignature: {
              // read string length
              uint8_t strLength = guts::endianness::ReadBe<uint8_t>(file);
              // event_.push_back(strLength);

#ifndef CXXMIDI_QUIET
              if ((meta_event_type == Event::SequenceNumber) &&
                  (strLength != 2))
                std::cerr << "CxxMidi: sequence number event size is not 2 but "
                          << strLength << std::endl;

              if ((meta_event_type == Event::ChannelPrefix) && (strLength != 1))
                std::cerr << "CxxMidi: channel prefix event size is not 1 but"
                          << strLength << std::endl;

              if ((meta_event_type == Event::OutputCable) && (strLength != 1))
                std::cerr << "CxxMidi: output cable event size is not 1 but"
                          << strLength << std::endl;

              if ((meta_event_type == Event::Tempo) && (strLength != 3))
                std::cerr << "CxxMidi: tempo event size is not 3 but"
                          << strLength << std::endl;

              if ((meta_event_type == Event::SmpteOffset) && (strLength != 5))
                std::cerr << "CxxMidi: SMPTE offset event size is not 5 but "
                          << strLength << std::endl;
#endif

              if (meta_event_type == 0x2f) {
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
          }
          break;
        }  // case 0xff, META events
        case Event::kSysExBegin:
        case Event::kSysExEnd: {
          uint32_t size = utils::GetVlq(file);
          // uint32_t size = Guts::Endianness::readBE<uint8_t>(file);

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
