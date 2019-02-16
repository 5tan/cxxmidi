#ifndef CXXMIDI_EVENT_HPP
#define CXXMIDI_EVENT_HPP

#include <cstdint>

#include <cxxmidi/message.hpp>

namespace cxxmidi {

class Event : public Message {
 public:
  inline Event();
  inline Event(uint32_t dt_);
  inline Event(uint32_t dt_, const Message &message_);
  inline Event(uint32_t dt_, uint8_t b1_);
  inline Event(uint32_t dt_, uint8_t b1_, uint8_t b2_);
  inline Event(uint32_t dt_, uint8_t b1_, uint8_t b2_, uint8_t b3_);

  inline uint32_t Dt() const;
  inline void SetDt(uint32_t dt_);

 private:
  uint32_t _dt;  // delta time [ticks]
};

}  // namespace CxxMidi

namespace cxxmidi {

Event::Event() {}

Event::Event(uint32_t dt_) : _dt(dt_) {}

Event::Event(uint32_t dt_, const Message &message_)
    : Message(message_), _dt(dt_) {}

Event::Event(uint32_t dt_, uint8_t b1_) : _dt(dt_) { this->push_back(b1_); }

Event::Event(uint32_t dt_, uint8_t b1_, uint8_t b2_) : _dt(dt_) {
  this->reserve(2);
  this->push_back(b1_);
  this->push_back(b2_);
}

Event::Event(uint32_t dt_, uint8_t b1_, uint8_t b2_, uint8_t b3_) : _dt(dt_) {
  this->reserve(3);
  this->push_back(b1_);
  this->push_back(b2_);
  this->push_back(b3_);
}

uint32_t Event::Dt() const { return _dt; }

void Event::SetDt(uint32_t dt_) { _dt = dt_; }

}  // namespace CxxMidi

#endif  // CXXMIDI_EVENT_HPP
