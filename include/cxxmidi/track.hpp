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

#ifndef INCLUDE_CXXMIDI_TRACK_HPP_
#define INCLUDE_CXXMIDI_TRACK_HPP_

#include <string>
#include <vector>

#include <cxxmidi/event.hpp>

namespace cxxmidi {

class Track : public std::vector<Event> {
 public:
  inline Event& AddEvent();

  inline std::string GetName() const;
};

}  // namespace cxxmidi

namespace cxxmidi {

Event& Track::AddEvent() {
  push_back(Event());
  return back();
}

std::string Track::GetName() const {
  std::string r;
  for (const auto& event : *this) {
    if (event.IsMeta()) {
      if (event.at(1) == Event::kTrackName) {
        if (!r.empty()) r += ", ";
        r += event.GetText();
      }
    }
  }
  return r;
}

}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_TRACK_HPP_
