#ifndef CXXMIDI_EVENT_HPP
#define CXXMIDI_EVENT_HPP

#include <cstdint>

#include <cxxmidi/message.hpp>

namespace cxxmidi {

class Event : public Message {
 public:
  inline Event();
  inline Event(uint32_t dt);
  inline Event(uint32_t dt, const Message &message);
  inline Event(uint32_t dt, uint8_t b1);
  inline Event(uint32_t dt, uint8_t b1, uint8_t b2);
  inline Event(uint32_t dt, uint8_t b1, uint8_t b2, uint8_t b3);

  inline uint32_t Dt() const;
  inline void SetDt(uint32_t dt);

 private:
  uint32_t _dt;  // delta time [ticks]
};

}  // namespace CxxMidi

namespace cxxmidi {

Event::Event() {}

Event::Event(uint32_t dt) : _dt(dt) {}

Event::Event(uint32_t dt, const Message &message)
    : Message(message), _dt(dt) {}

Event::Event(uint32_t dt, uint8_t b1) : _dt(dt) { this->push_back(b1); }

Event::Event(uint32_t dt, uint8_t b1, uint8_t b2) : _dt(dt) {
  this->reserve(2);
  this->push_back(b1);
  this->push_back(b2);
}

Event::Event(uint32_t dt, uint8_t b1, uint8_t b2, uint8_t b3) : _dt(dt) {
  this->reserve(3);
  this->push_back(b1);
  this->push_back(b2);
  this->push_back(b3);
}

uint32_t Event::Dt() const { return _dt; }

void Event::SetDt(uint32_t dt) { _dt = dt; }

}  // namespace CxxMidi

#endif  // CXXMIDI_EVENT_HPP
