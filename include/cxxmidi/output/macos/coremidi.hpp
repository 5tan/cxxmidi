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

#ifndef INCLUDE_CXXMIDI_OUTPUT_MACOS_COREMIDI_HPP_
#define INCLUDE_CXXMIDI_OUTPUT_MACOS_COREMIDI_HPP_

#include <CoreAudio/HostTime.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreMIDI/CoreMIDI.h>

#include <cstdint>
#include <cxxmidi/output/abstract.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace cxxmidi {
namespace output {
namespace macos {

class CoreMidi : public output::Abstract {
  // A structure to hold variables related to the CoreMIDI API
  // implementation.
  struct CoreMidiData {
    MIDIClientRef client;
    MIDIPortRef port;
    MIDIEndpointRef endpoint;
    MIDIEndpointRef destinationId;
  };

 public:
  inline CoreMidi();
  inline explicit CoreMidi(unsigned int initial_port);
  inline virtual ~CoreMidi();

  CoreMidi(const CoreMidi&);             // non-copyable
  CoreMidi& operator=(const CoreMidi&);  // non-copyable (assignment)

  inline void OpenPort(unsigned int port_num = 0) override;
  inline void ClosePort() override;
  inline void OpenVirtualPort(
      const std::string& port_name = std::string("CxxMidi Output")) override;
  inline size_t GetPortCount() override;
  inline std::string GetPortName(unsigned int port_num = 0) override;
  inline void SendMessage(const std::vector<uint8_t>* msg) override;

 protected:
  inline void Initialize() override;

 private:
  CoreMidiData* _apiData;
};

}  // namespace macos
}  // namespace output
}  // namespace cxxmidi

namespace cxxmidi {
namespace output {
namespace macos {

CoreMidi::CoreMidi() : _apiData(0) { Initialize(); }

CoreMidi::CoreMidi(unsigned int initial_port) : _apiData(0) {
  Initialize();
  OpenPort(initial_port);
}

CoreMidi::~CoreMidi() {
  // Close a connection if it exists.
  ClosePort();

  // Cleanup.
  if (_apiData->endpoint) MIDIEndpointDispose(_apiData->endpoint);
  if (_apiData->client) MIDIClientDispose(_apiData->client);
  delete _apiData;
}

size_t CoreMidi::GetPortCount() { return MIDIGetNumberOfDestinations(); }

std::string CoreMidi::GetPortName(unsigned int port_num) {
  std::string string_name;

  CFStringRef name_ref;
  MIDIEndpointRef port_ref;
  char name[128];

  port_ref = MIDIGetDestination(port_num);
  if (port_ref == 0) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: invalid port number " << port_num << std::endl;
#endif
    return string_name;
  }

  MIDIObjectGetStringProperty(port_ref, kMIDIPropertyName, &name_ref);
  CFStringGetCString(name_ref, name, sizeof(name), kCFStringEncodingUTF8);
  CFRelease(name_ref);

  string_name = name;
  return string_name;
}

void CoreMidi::Initialize() {
  // Set up the CoreMIDI client
  _apiData = new CoreMidiData;
  _apiData->client = 0;
  _apiData->port = 0;
  _apiData->endpoint = 0;
  _apiData->destinationId = 0;

  CFStringRef client_name = CFStringCreateWithCString(
      NULL, "CxxMidi (RtMidi) Output Client", kCFStringEncodingUTF8);
  OSStatus result =
      MIDIClientCreate(client_name, NULL, NULL, &_apiData->client);
  CFRelease(client_name);

#ifndef CXXMIDI_QUIET
  if (result != noErr) {
    std::cerr << "CxxMidi: CoreMIDI client init error" << std::endl;
    delete _apiData;
    _apiData = 0;
    return;
  }
#endif

  // We'll issue a warning here if no devices are available but not
  // throw an error since the user can plug something in later.
  size_t nDevices = GetPortCount();
#ifndef CXXMIDI_QUIET
  if (nDevices == 0)
    std::cerr << "CxxMidi: no MIDI output destinations available" << std::endl;
#endif
}

void CoreMidi::OpenPort(unsigned int port_num) {
  if (connected_) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: a valid connection already exists" << std::endl;
#endif
    return;
  }

  size_t nDevices = GetPortCount();
  if (port_num >= nDevices) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: invalid port number " << port_num << std::endl;
#endif
    return;
  }

  MIDIEndpointRef destination = MIDIGetDestination(port_num);
  if (destination == 0) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error getting MIDI destination" << std::endl;
#endif
    return;
  }

  // Create a MIDI output port
  CFStringRef port_name = CFStringCreateWithCString(
      NULL, "CxxMidi (RtMidi) Output Port", kCFStringEncodingUTF8);
  OSStatus result =
      MIDIOutputPortCreate(_apiData->client, port_name, &_apiData->port);
  CFRelease(port_name);

  if (result != noErr) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error creating MIDI output port" << std::endl;
#endif
    return;
  }

  _apiData->destinationId = destination;
  connected_ = true;
}

void CoreMidi::ClosePort() {
  if (connected_) {
    if (_apiData->port) {
      MIDIPortDispose(_apiData->port);
      _apiData->port = 0;
    }
    _apiData->destinationId = 0;
    connected_ = false;
  }
}

void CoreMidi::OpenVirtualPort(const std::string& port_name) {
  if (_apiData->endpoint) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: virtual output port already exists" << std::endl;
#endif
    return;
  }

  // Create a virtual destination
  CFStringRef name_ref =
      CFStringCreateWithCString(NULL, port_name.c_str(), kCFStringEncodingUTF8);
  OSStatus result =
      MIDISourceCreate(_apiData->client, name_ref, &_apiData->endpoint);
  CFRelease(name_ref);

  if (result != noErr) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error creating virtual MIDI source" << std::endl;
#endif
    return;
  }
}

void CoreMidi::SendMessage(const std::vector<uint8_t>* msg) {
  unsigned int nBytes = static_cast<unsigned int>(msg->size());
  if (nBytes == 0) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: message argument is empty" << std::endl;
#endif
    return;
  }

  // Check if we need to use a packet list (for connected port) or
  // MIDIReceived (for virtual endpoint)
  MIDIPacketList packet_list;
  MIDIPacket* packet = MIDIPacketListInit(&packet_list);

  // Use current time (0 means send immediately)
  MIDITimeStamp time_stamp = 0;

  packet = MIDIPacketListAdd(&packet_list, sizeof(packet_list), packet,
                             time_stamp, nBytes, msg->data());

  if (packet == NULL) {
#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: error creating MIDI packet" << std::endl;
#endif
    return;
  }

  OSStatus result;

  // Send to a connected port
  if (connected_ && _apiData->destinationId) {
    result = MIDISend(_apiData->port, _apiData->destinationId, &packet_list);
    if (result != noErr) {
#ifndef CXXMIDI_QUIET
      std::cerr << "CxxMidi: error sending MIDI message" << std::endl;
#endif
    }
  }
  // Or send to virtual endpoint
  else if (_apiData->endpoint) {
    result = MIDIReceived(_apiData->endpoint, &packet_list);
    if (result != noErr) {
#ifndef CXXMIDI_QUIET
      std::cerr << "CxxMidi: error sending MIDI message to virtual port"
                << std::endl;
#endif
    }
  }
#ifndef CXXMIDI_QUIET
  else {
    std::cerr << "CxxMidi: no valid output connection" << std::endl;
  }
#endif
}

}  // namespace macos
}  // namespace output
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_OUTPUT_MACOS_COREMIDI_HPP_
