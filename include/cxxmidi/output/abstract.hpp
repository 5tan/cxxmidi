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

#ifndef INCLUDE_CXXMIDI_OUTPUT_ABSTRACT_HPP_
#define INCLUDE_CXXMIDI_OUTPUT_ABSTRACT_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace cxxmidi {
namespace output {

class Abstract {
 public:
  inline Abstract();
  virtual ~Abstract() = default;

  Abstract(const Abstract &);             // non-copyable
  Abstract &operator=(const Abstract &);  // non-copyable (assignment)

  virtual void OpenPort(unsigned int port_num = 0) = 0;
  virtual void ClosePort() = 0;
  virtual void OpenVirtualPort(
      const std::string &port_name = std::string("RtMidi Virtual Output")) = 0;
  virtual size_t GetPortCount() = 0;
  virtual std::string GetPortName(unsigned int port_num = 0) = 0;
  virtual void SendMessage(const std::vector<uint8_t> *msg) = 0;
  inline void Reset();

 protected:
  inline virtual void Initialize() = 0;
  bool connected_;
};

}  // namespace output
}  // namespace cxxmidi

namespace cxxmidi {
namespace output {

Abstract::Abstract() : connected_(false) {}

void Abstract::Reset() {
  std::vector<uint8_t> message(3);
  message[2] = 0;

  for (int i = 176; i <= 191; i++) {
    message[0] = i;
    message[1] = 120;  // all sound off
    SendMessage(&message);

    message[1] = 121;  // reset all controllers
    SendMessage(&message);

    message[1] = 122;  // local control off
    SendMessage(&message);

    message[1] = 123;  // all notes off
    SendMessage(&message);
  }
}

}  // namespace output
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_OUTPUT_ABSTRACT_HPP_
