#ifndef CXXMIDI_TIME_PERIOD_HPP
#define CXXMIDI_TIME_PERIOD_HPP

#include <utility>

#include <cxxmidi/time/point.hpp>

namespace cxxmidi {
namespace time {

class Duration;

class Period : public std::pair<Point, Point> {
 public:
  inline Period();
  inline Period(const Point& first_, const Point& second_);
  inline Duration duration() const;

 private:
};

}  // namespace Time
}  // namespace CxxMidi

#include <ostream>

#include <cxxmidi/time/duration.hpp>

inline std::ostream& operator<<(std::ostream& os_,
                                const cxxmidi::time::Period& tp_) {
  os_ << '[' << tp_.first.toTimecode(true) << '-' << tp_.second.toTimecode(true)
      << ']';
  return os_;
}

namespace cxxmidi {
namespace time {

Period::Period() {}

Period::Period(const Point& first_, const Point& second_) {
  this->first = first_;
  this->second = second_;
}

Duration Period::duration() const { return Duration(*this); }

}  // namespace Time
}  // namespace CxxMidi

#endif  // CXXMIDI_TIME_PERIOD_HPP
