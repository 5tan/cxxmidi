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

#ifndef INCLUDE_CXXMIDI_EVENT_HPP_
#define INCLUDE_CXXMIDI_EVENT_HPP_

#include <cstdint>

#include <cxxmidi/message.hpp>

namespace cxxmidi {

class Event : public Message {
 public:
  inline Event() = default;
  inline explicit Event(uint32_t dt);
  inline explicit Event(uint32_t dt, const Message &message);
  inline explicit Event(uint32_t dt, uint8_t b1);
  inline explicit Event(uint32_t dt, uint8_t b1, uint8_t b2);
  inline explicit Event(uint32_t dt, uint8_t b1, uint8_t b2, uint8_t b3);

  inline uint32_t Dt() const;
  inline void SetDt(uint32_t dt);

 private:
  uint32_t dt_;  // delta time [ticks]
};

}  // namespace cxxmidi

namespace cxxmidi {

Event::Event(uint32_t dt) : dt_(dt) {}

Event::Event(uint32_t dt, const Message &message) : Message(message), dt_(dt) {}

Event::Event(uint32_t dt, uint8_t b1) : dt_(dt) { push_back(b1); }

Event::Event(uint32_t dt, uint8_t b1, uint8_t b2) : Message(b1, b2), dt_(dt) {}

Event::Event(uint32_t dt, uint8_t b1, uint8_t b2, uint8_t b3)
    : Message(b1, b2, b3), dt_(dt) {}

uint32_t Event::Dt() const { return dt_; }

void Event::SetDt(uint32_t dt) { dt_ = dt; }

}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_EVENT_HPP_
