#ifndef CXXMIDI_TIME_DURATION_HPP
#define CXXMIDI_TIME_DURATION_HPP

#include <cxxmidi/time/point.hpp>

namespace cxxmidi {
namespace time {


class Duration {
 public:
  inline Duration();
  inline Duration(const Point& p1, const Point& p2);

  inline void AddS(int s);
  inline void AddMs(int ms);
  inline void AddUs(int us);

  inline Point ToPoint() const;
  inline std::string ToTimecode(bool includeUs = false) const;

 private:
  Point point_;
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

Duration::Duration(const Point& p1, const Point& p2) {
  point_ = p1 - p2;
  if (point_.Negative()) point_.FlipSign();
}

Point Duration::ToPoint() const { return point_; }

std::string Duration::ToTimecode(bool includeUs) const {
  return point_.ToTimecode(includeUs);
}

void Duration::AddS(int s) { point_ += Point::FromS(s); }

void Duration::AddMs(int ms) { point_ += Point::FromMs(ms); }

void Duration::AddUs(int us) { point_ += Point::FromUs(us); }

}  // namespace Time
}  // namespace CxxMidi

#endif  // CXXMIDI_TIME_DURATION_HPP
