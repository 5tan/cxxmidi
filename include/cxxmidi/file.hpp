#ifndef CXXMIDI_FILE_HPP
#define CXXMIDI_FILE_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include <cxxmidi/guts/stdint.hpp>

#include <cxxmidi/track.hpp>

namespace CxxMidi {
namespace Time {
class Duration;
} // namespace Time

class File : public std::vector<Track>
{
public:
    inline File();
    inline File(const char *path_);

    inline void load(const char *path_);
    inline void saveAs(const char *path_) const;

    inline Track& addTrack();
    inline size_t tracks() const;

    inline uint16_t timeDivision() const;
    inline void setTimeDivision(uint16_t timeDivision_);

    inline Time::Duration duration() const;

private:

    uint16_t _timeDivision; // [ticks per quarternote]

    inline void readHeaderChunk(std::fstream & file_);
    inline void readUnknownChunk(std::fstream & file_);
    inline void readTrackChunk(std::fstream & file_);
    inline void readEvent(std::fstream & file_,
                          Event &event_,
                          bool &trackContinue_,
                          uint8_t &runningStatus_);

    inline void saveHeaderChunk(std::ofstream &outputFile_) const;
    inline void saveTrackChunk(std::ofstream & outputFile_,
                               size_t num_) const;
    inline size_t saveEvent(std::ofstream & outputFile_,
                            const Event & event_,
                            uint8_t & lastCmd_) const;

    inline static void putc(std::ofstream& file_, uint8_t c_);

};

} // namespace CxxMidi

#include <cxxmidi/guts/endianness.hpp>
#include <cxxmidi/utils.hpp>
#include <cxxmidi/guts/simulator.hpp>
#include <cxxmidi/time/duration.hpp>

namespace CxxMidi {

void File::putc(std::ofstream& file_, uint8_t c_)
{
    file_.write(reinterpret_cast<char*>(&c_),1);
}

File::File()
    : _timeDivision(500)
{
}

File::File(const char* path_)
    : _timeDivision(500)
{
    this->load(path_);
}

void File::saveAs(const char * path_) const
{
    std::ofstream outputFile(path_, std::ios_base::binary);

    if(!outputFile.good())
    {
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: could not open file " << path_ << std::endl;
#endif
        return;
    }

    // save header
    this->saveHeaderChunk(outputFile);

    // loop over tracks
    for(unsigned int i=0; i<this->tracks(); i++)
        this->saveTrackChunk(outputFile,i);

    outputFile.close();
}

void File::saveTrackChunk(std::ofstream & outputFile_,
                          size_t num_) const
{
    const Track & track = this->at(num_);

    // save chunk id
    Guts::Endianness::writeBE<uint32_t>(outputFile_,0x4D54726B); // "MTrk"

    // write dummy chunk size (we will get back here)
    std::streampos sizePos = outputFile_.tellp();
    Guts::Endianness::writeBE<uint32_t>(outputFile_,0);

    uint32_t chunkSize=0; // chunk size
    uint8_t lastCmd =0;

    for(size_t i=0; i<track.size() ; i++)
    {
        const Event & event = track.at(i);
        chunkSize += static_cast<uint32_t>(
                    this->saveEvent(outputFile_,event,lastCmd));
    }

    // go back to chunk size
    outputFile_.seekp(sizePos);
    // save chunk size
    Guts::Endianness::writeBE<uint32_t>(outputFile_,chunkSize);

    // go to end of file
    outputFile_.seekp(0,std::ios_base::end);
}

size_t File::saveEvent(std::ofstream & outputFile_,
                       const Event & event_,
                       uint8_t & lastCmd_) const
{
    size_t r=0;

    if(event_.isSysex())
    {
        // save delta time variable-length quantity
        r += Utils::saveVlq(outputFile_,event_.dt());

        File::putc(outputFile_,event_.at(0)); // SysEx type
        r++;

        uint8_t dataSize = static_cast<uint8_t>(event_.size()) - 1;
        r += Utils::saveVlq(outputFile_,dataSize);

        //! @TODO check it
        for(size_t i=1; i< event_.size(); i++)
        {
            File::putc(outputFile_,event_.at(i)); // save data bytes
            r++;
        }

        lastCmd_ = 0;

        return r;
    }

    if(event_.isMeta())
    {
        // save delta time variable-length quantity
        r += Utils::saveVlq(outputFile_,event_.dt());

        File::putc(outputFile_,0xff); // byte 0 (event_.at(0))
        File::putc(outputFile_,event_.at(1)); // byte 1, meta event type
        r+=2;

        uint8_t dataSize = static_cast<uint8_t>(event_.size()) - 2;
        File::putc(outputFile_,dataSize); // save length
        r++;

        for(size_t i=2; i< event_.size(); i++)
        {
            File::putc(outputFile_,event_.at(i)); // save data bytes
            r++;
        }

        lastCmd_ = 0;

        return r;
    }

    uint8_t nowCmd = event_.at(0);

    r += Utils::saveVlq(outputFile_,event_.dt());

    if(lastCmd_ != nowCmd)
    {
        File::putc(outputFile_,nowCmd); // byte 0, event type
        r++;
    }

    for(size_t i=1; i<event_.size(); i++)
    {
        File::putc(outputFile_,event_.at(i)); // data bytes
        r++;
    }

    // save current command if we use running status
    lastCmd_ = nowCmd;

    return r;
}

void File::saveHeaderChunk(std::ofstream &outputFile_) const
{
    // save chunk id
    Guts::Endianness::writeBE<uint32_t>(outputFile_,0x4D546864); // "MThd"

    // save header size
    Guts::Endianness::writeBE<uint32_t>(outputFile_,6);

    // save file type (dummy, real value saved later)
    uint16_t fileType = (this->tracks() >1) ? 1 : 0;
    Guts::Endianness::writeBE<uint16_t>(outputFile_,fileType);

    // save tracks numer (dummy, real value saved later)
    Guts::Endianness::writeBE<uint16_t>(outputFile_,
                                        static_cast<uint16_t>(this->tracks()));

    // save time division
    Guts::Endianness::writeBE<uint16_t>(outputFile_,_timeDivision);
}

Time::Duration File::duration() const
{
    Guts::Simulator simulator;
    return simulator.duration(*this);
}

Track& File::addTrack()
{
    this->push_back(Track());
    return this->back();
}

size_t File::tracks() const
{
    return this->size();
}

uint16_t File::timeDivision() const
{
    return _timeDivision;
}

void File::setTimeDivision(uint16_t timeDivision_)
{
    _timeDivision = timeDivision_;
}

void File::load(const char *path_)
{
    this->clear();

    // open file
    std::fstream file(path_,std::fstream::in | std::fstream::binary);

    if(!file.is_open())
    {
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: couldn't open: " << path_ << std::endl;
#endif
        return;
    }

    // calculate file length
    file.seekg (0, std::fstream::end);
    std::fstream::streampos fileLength = file.tellg();
    file.seekg (0, std::fstream::beg);

    // control counters
    unsigned int i=0;
    unsigned int headers=0;

    // loop over chunks
    while(file.good() && (fileLength-file.tellg())) // data still in buffer
    {
        uint32_t chunkId = Guts::Endianness::readBE<uint32_t>(file);

        switch(chunkId)
        {
        case 0x4D546864: // "MThd"
            this->readHeaderChunk(file);
            headers++;
            break;

        case 0x4D54726B: // "MTrk"
            this->readTrackChunk(file);
            break;

        default:
#ifndef CXXMIDI_QUIET
            std::cerr << "CxxMidi: ignoring unknown chunk: 0x"
                      << std::hex << (int)chunkId << std::endl;
#endif
            this->readUnknownChunk(file);
            break;
        }

        if(!i++ && !headers)
        {
#ifndef CXXMIDI_QUIET
            std::cerr << "CxxMidi: no header chunk "
                         "(probably not a MIDI file)" << std::endl;
#endif
            break;
        }
    }
}

void File::readHeaderChunk(std::fstream & file_)
{
    // read and check header size
    if( Guts::Endianness::readBE<uint32_t>(file_) != 6)
    {
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: warning: unsupported MIDI file type" << std::endl;
#endif
    }

    // read file type
    uint16_t type = Guts::Endianness::readBE<uint16_t>(file_);

#ifndef CXXMIDI_QUIET
    // check file type
    if( (type != 0) && (type != 1) )
        std::cerr << "CxxMidi: warning: unsupported MIDI file type: "
                  << type << std::endl;
#endif
    // type 0: single track
    // type 1: multi track

    // read tracks number
    uint16_t tracksNumber = Guts::Endianness::readBE<uint16_t>(file_);

    // reserve vector capacity
    this->reserve(tracksNumber);

    // read time division
    _timeDivision = Guts::Endianness::readBE<uint16_t>(file_);

#ifndef CXXMIDI_QUIET
    // check time division
    if (_timeDivision & 0x8000)
        std::cerr << "CxxMidi: warning: unsupported MIDI file time division" << std::endl;
#endif
}

void File::readUnknownChunk(std::fstream & file_)
{
    // get chunk size
    uint32_t chunkSize = Guts::Endianness::readBE<uint32_t>(file_);

    // skip chunk data
    file_.seekg(chunkSize,std::fstream::cur);
}

void File::readTrackChunk(std::fstream & file_)
{
    // push back new track
    Track &track = this->addTrack();

    // read track chunk size
    uint32_t chunkSize = Guts::Endianness::readBE<uint32_t>(file_);
    // we will not use this size to read data (we wait for end event)

    uint8_t runningStatus = 0; // start with no running status
    std::fstream::streampos begin = file_.tellg();
    bool trackContinue = true;

    // read track data
    while(trackContinue)
    {
        // create new event
        Event &event = track.addEvent();

        // get delta time
        uint32_t dt = Utils::getVlq(file_);

        // save event delta time
        event.setDt(dt);

        // read event data
        this->readEvent(file_,event,trackContinue,runningStatus);
    }

#ifndef CXXMIDI_QUIET
    if(chunkSize != (file_.tellg()-begin))
        std::cerr << "CxxMidi: warning: track data and track chunk size mismatch" << std::endl;
#endif
}

void File::readEvent(std::fstream & file_,
                     Event &event_,
                     bool &trackContinue_,
                     uint8_t &runningStatus_)
{
    uint8_t cmd = Guts::Endianness::readBE<uint8_t>(file_);

    // check running status
    bool incomplete = false;
    if(cmd < 0x80)
    {
        incomplete = true; // this flag says: do not read to much later
        event_.push_back(runningStatus_); // command from previous complete event
        event_.push_back(cmd);
        cmd = runningStatus_; // swap
    }
    else
    {
        runningStatus_ = cmd; // current command for this track
        event_.push_back(cmd);
    }

    switch (cmd & 0xf0) // control events
    {
    // two parameter events
    case Event::NoteOn:
    case Event::NoteOff:
    case Event::NoteAftertouch:
    case Event::ControlChange:
    case Event::PitchWheel:
    {
        // get parameter 1
        event_.push_back(Guts::Endianness::readBE<uint8_t>(file_));

        // get parameter 2
        if(!incomplete)
            event_.push_back(Guts::Endianness::readBE<uint8_t>(file_));
    }
        break;

        // one parameter events
    case Event::ProgramChange:
    case Event::ChannelAftertouch:
    {
        if(!incomplete)
            event_.push_back(Guts::Endianness::readBE<uint8_t>(file_));
    }
        break;

    case 0xf0: // META events or SysEx events
    {
        switch (cmd)
        {
        case 0xff: // META events
        {
            uint8_t metaEventType = Guts::Endianness::readBE<uint8_t>(file_);
            event_.push_back(metaEventType);

            switch(metaEventType)
            {
            default:
            {
#ifndef CXXMIDI_QUIET
                std::cerr << "CxxMidi: unknown meta event 0x"
                          << std::hex << metaEventType << std::endl;
#endif
            }
            case Event::SequenceNumber: // size always 2
            case Event::Text:
            case Event::Copyright:
            case Event::TrackName:
            case Event::InstrumentName:
            case Event::Lyrics:
            case Event::Marker:
            case Event::CuePoint:
            case Event::ChannelPrefix: // size always 1
            case Event::OutputCable: //size always 1
            case Event::EndOfTrack: // size always 0
            case Event::Tempo: // size always 3
            case Event::SmpteOffset: // size always 5
            case Event::TimeSignature:
            case Event::KeySignature:
            {
                // read string length
                uint8_t strLength = Guts::Endianness::readBE<uint8_t>(file_);
                //event_.push_back(strLength);

#ifndef CXXMIDI_QUIET
                if((metaEventType == Event::SequenceNumber)
                        && (strLength != 2))
                    std::cerr << "CxxMidi: sequence number event size is not 2 but " << strLength << std::endl;

                if((metaEventType == Event::ChannelPrefix)
                        && (strLength != 1))
                    std::cerr << "CxxMidi: channel prefix event size is not 1 but" <<  strLength << std::endl;

                if((metaEventType == Event::OutputCable)
                        && (strLength != 1))
                    std::cerr << "CxxMidi: output cable event size is not 1 but" << strLength << std::endl;

                if((metaEventType == Event::Tempo)
                        && (strLength != 3))
                    std::cerr << "CxxMidi: tempo event size is not 3 but" << strLength << std::endl;

                if((metaEventType == Event::SmpteOffset)
                        && (strLength != 5))
                    std::cerr << "CxxMidi: SMPTE offset event size is not 5 but " << strLength << std::endl;
#endif

                if(metaEventType == 0x2f)
                {
#ifndef CXXMIDI_QUIET
                    if (strLength !=0)
                        std::cerr << "CxxMidi: end of track event size is not 0 but " << strLength << std::endl;
#endif
                    trackContinue_ = false;
                }

                // read string
                for(int i=0;i<strLength;i++)
                    event_.push_back(Guts::Endianness::readBE<uint8_t>(file_));
            }
                break;
            }
            break;
        } // case 0xff, META events
        case Event::SysExBegin:
        case Event::SysExEnd:
        {
            uint32_t size = Utils::getVlq(file_);
            //uint32_t size = Guts::Endianness::readBE<uint8_t>(file_);

            for(unsigned int i=0; i<size; i++)
                event_.push_back(Guts::Endianness::readBE<uint8_t>(file_));
        }
            break;
        } // switch cmd
    } // case 0xf0
        break;
    default:
#ifndef CXXMIDI_QUIET
        std::cerr << "warning: unknown event " << (int)cmd << std::endl;
#endif
        break;
    }
}

} // namespace CxxMidi

#endif // CXXMIDI_FILE_HPP
