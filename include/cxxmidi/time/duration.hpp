#ifndef CXXMIDI_TIME_DURATION_HPP
#define CXXMIDI_TIME_DURATION_HPP

#include <cxxmidi/time/point.hpp>

namespace cxxmidi {
namespace time {


class Duration {
 public:
  inline Duration();
  inline Duration(const Point& p1_, const Point& p2_);

  inline void AddS(int s_);
  inline void AddMs(int ms_);
  inline void AddUs(int us_);

  inline Point ToPoint() const;
  inline std::string ToTimecode(bool includeUs_ = false) const;

 private:
  Point _point;
};

}  // namespace Time
}  // namespace CxxMidi

#include <ostream>

inline std::ostream& operator<<(std::ostream& os_,
                                const cxxmidi::time::Duration& td_) {
  os_ << td_.ToTimecode(true);
  return os_;
}

namespace cxxmidi {
namespace time {

Duration::Duration() {}

Duration::Duration(const Point& p1_, const Point& p2_) {
  _point = p1_ - p2_;
  if (_point.Negative()) _point.FlipSign();
}

Point Duration::ToPoint() const { return _point; }

std::string Duration::ToTimecode(bool includeUs_) const {
  return _point.ToTimecode(includeUs_);
}

void Duration::AddS(int s_) { _point += Point::FromS(s_); }

void Duration::AddMs(int ms_) { _point += Point::FromMs(ms_); }

void Duration::AddUs(int us_) { _point += Point::FromUs(us_); }

}  // namespace Time
}  // namespace CxxMidi

#endif  // CXXMIDI_TIME_DURATION_HPP
