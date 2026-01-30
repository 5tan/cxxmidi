/* *****************************************************************************
RtMidi: realtime MIDI i/o C++ classes
Copyright (c) 2003-2011 Gary P. Scavone

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

Any person wishing to distribute modifications to the Software is
requested to send the modifications to the original developer so that
they can be incorporated into the canonical version.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***************************************************************************** */

#ifndef INCLUDE_CXXMIDI_OUTPUT_MACOS_AUDIOUNIT_SYNTH_HPP_
#define INCLUDE_CXXMIDI_OUTPUT_MACOS_AUDIOUNIT_SYNTH_HPP_

#include <AudioToolbox/AudioToolbox.h>
#include <CoreAudio/CoreAudio.h>
#include <CoreFoundation/CoreFoundation.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <cxxmidi/output/abstract.hpp>

namespace cxxmidi {
namespace output {
namespace macos {

class AudioUnitSynth : public output::Abstract {
  // A structure to hold variables related to the AudioUnit API
  struct AudioUnitData {
    AUGraph graph;
    AudioUnit synth_unit;
  };

 public:
  inline AudioUnitSynth();
  inline explicit AudioUnitSynth(unsigned int initial_port);
  inline virtual ~AudioUnitSynth();

  AudioUnitSynth(const AudioUnitSynth&);  // non-copyable
  AudioUnitSynth& operator=(
      const AudioUnitSynth&);  // non-copyable (assignment)

  inline void OpenPort(unsigned int port_num = 0) override;
  inline void ClosePort() override;
  inline void OpenVirtualPort(const std::string& port_name =
                                  std::string("CxxMidi Synth Output")) override;
  inline size_t GetPortCount() override;
  inline std::string GetPortName(unsigned int port_num = 0) override;
  inline void SendMessage(const std::vector<uint8_t>* msg) override;

 protected:
  inline void Initialize() override;

 private:
  AudioUnitData* _apiData;
};

}  // namespace macos
}  // namespace output
}  // namespace cxxmidi

namespace cxxmidi {
namespace output {
namespace macos {

AudioUnitSynth::AudioUnitSynth() : _apiData(0) { Initialize(); }

AudioUnitSynth::AudioUnitSynth(unsigned int initial_port) : _apiData(0) {
  Initialize();
  OpenPort(initial_port);
}

AudioUnitSynth::~AudioUnitSynth() {
  // Close a connection if it exists.
  ClosePort();

  // Cleanup.
  if (_apiData) {
    if (_apiData->graph) {
      AUGraphStop(_apiData->graph);
      AUGraphUninitialize(_apiData->graph);
      AUGraphClose(_apiData->graph);
      DisposeAUGraph(_apiData->graph);
    }
    delete _apiData;
  }
}

size_t AudioUnitSynth::GetPortCount() {
  // AudioUnit synth is always available as a single "port"
  return 1;
}

std::string AudioUnitSynth::GetPortName(unsigned int port_num) {
  if (port_num == 0) {
    return "AudioUnit DLS Synth";
  }
#ifndef CXXMIDI_QUIET
  std::cerr << "CxxMidi: invalid port number " << port_num << std::endl;
#endif
  return "";
}

void AudioUnitSynth::Initialize() {
  _apiData = new AudioUnitData;
  _apiData->graph = 0;
  _apiData->synth_unit = 0;

  OSStatus result;

  // Create AUGraph
  result = NewAUGraph(&_apiData->graph);
  if (result != noErr) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error creating AUGraph" << std::endl;
#endif
    delete _apiData;
    _apiData = 0;
    return;
  }

  // Add DLS synth node
  AudioComponentDescription synth_desc;
  synth_desc.componentType = kAudioUnitType_MusicDevice;
  synth_desc.componentSubType = kAudioUnitSubType_DLSSynth;
  synth_desc.componentManufacturer = kAudioUnitManufacturer_Apple;
  synth_desc.componentFlags = 0;
  synth_desc.componentFlagsMask = 0;

  AUNode synth_node;
  result = AUGraphAddNode(_apiData->graph, &synth_desc, &synth_node);
  if (result != noErr) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error adding synth node" << std::endl;
#endif
    DisposeAUGraph(_apiData->graph);
    delete _apiData;
    _apiData = 0;
    return;
  }

  // Add output node
  AudioComponentDescription output_desc;
  output_desc.componentType = kAudioUnitType_Output;
  output_desc.componentSubType = kAudioUnitSubType_DefaultOutput;
  output_desc.componentManufacturer = kAudioUnitManufacturer_Apple;
  output_desc.componentFlags = 0;
  output_desc.componentFlagsMask = 0;

  AUNode output_node;
  result = AUGraphAddNode(_apiData->graph, &output_desc, &output_node);
  if (result != noErr) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error adding output node" << std::endl;
#endif
    DisposeAUGraph(_apiData->graph);
    delete _apiData;
    _apiData = 0;
    return;
  }

  // Open the graph
  result = AUGraphOpen(_apiData->graph);
  if (result != noErr) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error opening AUGraph" << std::endl;
#endif
    DisposeAUGraph(_apiData->graph);
    delete _apiData;
    _apiData = 0;
    return;
  }

  // Get the synth unit
  result =
      AUGraphNodeInfo(_apiData->graph, synth_node, NULL, &_apiData->synth_unit);
  if (result != noErr) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error getting synth unit" << std::endl;
#endif
    AUGraphClose(_apiData->graph);
    DisposeAUGraph(_apiData->graph);
    delete _apiData;
    _apiData = 0;
    return;
  }

  // Connect synth to output
  result =
      AUGraphConnectNodeInput(_apiData->graph, synth_node, 0, output_node, 0);
  if (result != noErr) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error connecting nodes" << std::endl;
#endif
    AUGraphClose(_apiData->graph);
    DisposeAUGraph(_apiData->graph);
    delete _apiData;
    _apiData = 0;
    return;
  }

  // Initialize the graph
  result = AUGraphInitialize(_apiData->graph);
  if (result != noErr) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error initializing AUGraph" << std::endl;
#endif
    AUGraphClose(_apiData->graph);
    DisposeAUGraph(_apiData->graph);
    delete _apiData;
    _apiData = 0;
    return;
  }
}

void AudioUnitSynth::OpenPort(unsigned int port_num) {
  if (port_num != 0) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: invalid port number " << port_num << std::endl;
#endif
    return;
  }

  if (connected_) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: a valid connection already exists" << std::endl;
#endif
    return;
  }

  if (!_apiData || !_apiData->graph) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: synth not initialized" << std::endl;
#endif
    return;
  }

  // Start the graph
  OSStatus result = AUGraphStart(_apiData->graph);
  if (result != noErr) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error starting AUGraph" << std::endl;
#endif
    return;
  }

  connected_ = true;
}

void AudioUnitSynth::ClosePort() {
  if (connected_ && _apiData && _apiData->graph) {
    AUGraphStop(_apiData->graph);
    connected_ = false;
  }
}

void AudioUnitSynth::OpenVirtualPort(const std::string& /*port_name*/) {
  // Virtual ports don't make sense for a synth output
#ifndef CXXMIDI_QUIET
  std::cerr << "CxxMidi: AudioUnit synth doesn't support virtual ports"
            << std::endl;
#endif
}

void AudioUnitSynth::SendMessage(const std::vector<uint8_t>* msg) {
  if (!connected_ || !_apiData || !_apiData->synth_unit) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: synth not connected" << std::endl;
#endif
    return;
  }

  unsigned int nBytes = static_cast<unsigned int>(msg->size());
  if (nBytes == 0) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: message argument is empty" << std::endl;
#endif
    return;
  }

  OSStatus result;

  // Parse MIDI message
  uint8_t status = msg->at(0);
  uint8_t channel = status & 0x0F;
  uint8_t command = status & 0xF0;

  if (command == 0x80 || command == 0x90) {
    // Note off / Note on
    if (nBytes < 3) return;
    uint8_t note = msg->at(1);
    uint8_t velocity = msg->at(2);

    if (command == 0x90 && velocity > 0) {
      result = MusicDeviceMIDIEvent(_apiData->synth_unit, 0x90 | channel, note,
                                    velocity, 0);
    } else {
      result = MusicDeviceMIDIEvent(_apiData->synth_unit, 0x80 | channel, note,
                                    velocity, 0);
    }
  } else if (command == 0xB0) {
    // Control change
    if (nBytes < 3) return;
    uint8_t controller = msg->at(1);
    uint8_t value = msg->at(2);
    result = MusicDeviceMIDIEvent(_apiData->synth_unit, 0xB0 | channel,
                                  controller, value, 0);
  } else if (command == 0xC0) {
    // Program change
    if (nBytes < 2) return;
    uint8_t program = msg->at(1);
    result = MusicDeviceMIDIEvent(_apiData->synth_unit, 0xC0 | channel, program,
                                  0, 0);
  } else if (command == 0xE0) {
    // Pitch bend
    if (nBytes < 3) return;
    uint8_t lsb = msg->at(1);
    uint8_t msb = msg->at(2);
    result =
        MusicDeviceMIDIEvent(_apiData->synth_unit, 0xE0 | channel, lsb, msb, 0);
  } else if (command == 0xD0) {
    // Channel pressure
    if (nBytes < 2) return;
    uint8_t pressure = msg->at(1);
    result = MusicDeviceMIDIEvent(_apiData->synth_unit, 0xD0 | channel,
                                  pressure, 0, 0);
  } else if (command == 0xA0) {
    // Polyphonic key pressure
    if (nBytes < 3) return;
    uint8_t note = msg->at(1);
    uint8_t pressure = msg->at(2);
    result = MusicDeviceMIDIEvent(_apiData->synth_unit, 0xA0 | channel, note,
                                  pressure, 0);
  } else if (status == 0xF0) {
    // System exclusive
    result = MusicDeviceSysEx(_apiData->synth_unit, msg->data(), nBytes);
  } else {
    // Other messages - try sending as raw MIDI event
    if (nBytes >= 2) {
      result = MusicDeviceMIDIEvent(_apiData->synth_unit, msg->at(0),
                                    nBytes >= 2 ? msg->at(1) : 0,
                                    nBytes >= 3 ? msg->at(2) : 0, 0);
    } else {
      result = noErr;  // Ignore single-byte messages we don't handle
    }
  }

#ifndef CXXMIDI_QUIET
  if (result != noErr) {
    std::cerr << "CxxMidi: error sending MIDI event" << std::endl;
  }
#endif
}

}  // namespace macos
}  // namespace output
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_OUTPUT_MACOS_AUDIOUNIT_SYNTH_HPP_
