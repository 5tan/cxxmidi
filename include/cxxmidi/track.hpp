#ifndef CXXMIDI_TRACK_HPP
#define CXXMIDI_TRACK_HPP

#include <string>
#include <vector>

#include <cxxmidi/event.hpp>

namespace cxxmidi {

class Track : public std::vector<Event> {
 public:
  inline Event& AddEvent();

  inline std::string GetName() const;

 private:
};

}  // namespace CxxMidi

#include <cxxmidi/event.hpp>

namespace cxxmidi {

Event& Track::AddEvent() {
  this->push_back(Event());
  return this->back();
}

std::string Track::GetName() const {
  std::string r;

  for (size_t i = 0; i < this->size(); i++) {
    const Event* event = &this->at(i);
    if (event->IsMeta())
      if (event->at(1) == Event::TrackName) {
        if (!r.empty()) r += ", ";
        r += event->GetText();
      }
  }

  return r;
}

}  // namespace CxxMidi

#endif  // CXXMIDI_TRACK_HPP
