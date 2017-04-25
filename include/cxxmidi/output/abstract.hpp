// RtMidi: realtime MIDI i/o C++ classes
// Copyright (c) 2003-2011 Gary P. Scavone
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation files
// (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// Any person wishing to distribute modifications to the Software is
// requested to send the modifications to the original developer so that
// they can be incorporated into the canonical version.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef CXXMIDI_OUTPUT_ABSTRACT_HPP
#define CXXMIDI_OUTPUT_ABSTRACT_HPP

#include <string>
#include <vector>
#include <cxxmidi/guts/stdint.hpp>

namespace CxxMidi {
namespace Output {

class Abstract
{
public:

    inline Abstract();
    inline ~Abstract();

    Abstract(const Abstract&); // non-copyable
    Abstract &operator=(const Abstract &); // non-copyable (assignment)
#if __cplusplus > 199711L
    Abstract(Abstract&&) = default;
    Abstract& operator=(Abstract&&) = default;
#endif // __cplusplus > 199711L

    virtual void openPort(unsigned int portNumber_=0) =0;
    virtual void closePort() =0;
    virtual void openVirtualPort(const std::string& portName_ = std::string( "RtMidi Virtual Output") ) =0;
    virtual size_t getPortCount() =0;
    virtual std::string getPortName(unsigned int portNumber_ = 0 ) =0;
    virtual void sendMessage(const std::vector<uint8_t> *msg_ ) =0;
    inline void reset();

protected:

    inline virtual void initialize()=0;
    bool _connected;
};

} // namespace Output
} // namespace CxxMidi

#include <cxxmidi/guts/stdint.hpp>

namespace CxxMidi {
namespace Output {

Abstract::Abstract()
    : _connected(false)
{

}

Abstract::~Abstract()
{

}

void Abstract::reset()
{
    std::vector<uint8_t> message(3);
    message[2] = 0;

    for(int i=176; i<=191; i++)
    {
      message[0] = i;
      message[1] = 120;  // all sound off
      this->sendMessage(&message);

      message[1] = 121; // reset all controllers
      this->sendMessage(&message);

      message[1] = 122; // local control off
      this->sendMessage(&message);

      message[1] = 123; // all notes off
      this->sendMessage(&message);
    }
}

} // namespace Output
} // namespace CxxMidi

#endif // CXXMIDI_OUTPUT_ABSTRACT_HPP
