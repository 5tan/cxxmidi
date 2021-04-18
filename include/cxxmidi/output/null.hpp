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

#ifndef INCLUDE_CXXMIDI_OUTPUT_NULL_HPP_
#define INCLUDE_CXXMIDI_OUTPUT_NULL_HPP_

#include <string>
#include <vector>

#include <cxxmidi/output/abstract.hpp>

namespace cxxmidi {
namespace output {

class Null : public output::Abstract {
 public:
  inline Null() {}

  Null(const Null &) = delete;             // non-copyable
  Null &operator=(const Null &) = delete;  // non-copyable (assignment)

  inline void OpenPort(unsigned int port_num = 0) override {}
  inline void ClosePort() override {}
  inline void OpenVirtualPort(
      const std::string &port_name = std::string("RtMidi Output")) override {}
  inline size_t GetPortCount() override { return 1; }
  inline std::string GetPortName(unsigned int port_num = 0) override {
    return "null";
  }
  inline void SendMessage(const std::vector<uint8_t> * /* msg */) override {}

 protected:
  inline void Initialize() override {}
};

}  // namespace output
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_OUTPUT_NULL_HPP_
