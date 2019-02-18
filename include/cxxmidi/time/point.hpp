#ifndef CXXMIDI_TIME_POINT_HPP
#define CXXMIDI_TIME_POINT_HPP

#include <cstdint>

namespace cxxmidi {
namespace time {

class Point {
 public:
  inline Point();
  inline static Point FromUs(int us);
  inline static Point FromMs(int ms);
  inline static Point FromS(int s);
  inline static Point FromTimecode(const std::string& tc);
  inline static Point Zero();

  inline std::string ToTimecode(bool includeUs = false) const;

  inline bool Negative() const { return is_negative_; }
  inline void SetNegative(bool negative);
  inline void FlipSign();
  inline unsigned int S() { return s_; }
  inline unsigned int Us() { return us_; }

  inline void AddUs(int us);
  inline void AddMs(int ms);
  inline void AddS(int s);

  inline Point operator-() const;
  inline Point operator+(const Point& rhs) const;
  inline Point operator-(const Point& rhs) const;
  inline Point operator*(double factor) const;
  inline Point operator+=(const Point& rhs);
  inline Point operator-=(const Point& rhs);
  inline Point operator*=(double factor);
  inline bool operator==(const Point& rhs) const;
  inline bool operator!=(const Point& rhs) const;
  inline bool operator>(const Point& rhs) const;
  inline bool operator>=(const Point& rhs) const;
  inline bool operator<(const Point& rhs) const;
  inline bool operator<=(const Point& rhs) const;
  inline double operator/(const Point& rhs) const;

 private:
  bool is_negative_;
  uint32_t s_;
  int32_t us_;

  inline Point AddPositive(const Point& a, const Point& b) const;
  inline Point SubstractPositive(const Point& a, const Point& b) const;
  inline void FixZeroSign();
};

inline Point operator*(double lhs, const Point& rhs_);

}  // namespace Time
}  // namespace CxxMidi

#include <cassert>
#include <complex>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

inline std::ostream& operator<<(std::ostream& os_,
                                const cxxmidi::time::Point& tp_) {
  os_ << tp_.ToTimecode(true);
  return os_;
}

namespace cxxmidi {
namespace time {

Point::Point() : is_negative_(false), s_(0), us_(0) {}

std::string Point::ToTimecode(bool includeUs) const {
  std::stringstream ss;

  ss << (is_negative_ ? "-" : "") << std::setw(2) << std::setfill('0')
     << (s_ / (60 * 60)) << ":" << std::setw(2) << std::setfill('0')
     << ((s_ / 60) % 60) << ":" << std::setw(2) << std::setfill('0')
     << (s_ % 60);

  if (includeUs && us_) ss << "&" << us_;

  return ss.str();
}

Point Point::Zero() { return Point(); }

Point Point::FromTimecode(const std::string& tc) {
  Point r;
  int hh, mm, ss, us = 0;

  if (tc.find('&') == std::string::npos)
    std::sscanf(tc.c_str(), "%d:%d:%d", &hh, &mm, &ss);
  else {
    std::sscanf(tc.c_str(), "%d:%d:%d&%d", &hh, &mm, &ss, &us);
    r.us_ = us;
  }

  r.s_ = ss + (60 * mm) + (60 * 60 * hh);
  r.s_ += us / 1000000;
  r.us_ %= 1000000;

  return r;
}

Point Point::FromUs(int us) {
  Point r;

  if (us < 0) {
    r.is_negative_ = true;
    us = std::abs(us);
  }

  r.s_ = us / 1000000;
  r.us_ = us % 1000000;

  r.FixZeroSign();
  return r;
}

Point Point::FromMs(int ms) {
  Point r;

  if (ms < 0) {
    r.is_negative_ = true;
    ms = std::abs(ms);
  }

  r.s_ = ms / 1000;
  r.us_ = (ms % 1000) * 1000;

  r.FixZeroSign();
  return r;
}

Point Point::FromS(int s) {
  Point r;

  if (s < 0) {
    r.is_negative_ = true;
    s = std::abs(s);
  }

  r.s_ = s;

  r.FixZeroSign();
  return r;
}

void Point::AddUs(int us) {
  *this = *this + FromUs(us);
  this->FixZeroSign();
}

void Point::AddMs(int ms) {
  *this = *this + FromMs(ms);
  this->FixZeroSign();
}

void Point::AddS(int s) {
  *this = *this + FromS(s);
  this->FixZeroSign();
}

void Point::FixZeroSign() {
  if (!s_)
    if (!us_) is_negative_ = false;
}

void Point::FlipSign() {
  is_negative_ = !is_negative_;
  this->FixZeroSign();
}

void Point::SetNegative(bool negative) {
  is_negative_ = negative;
  this->FixZeroSign();
}

Point Point::operator-() const {
  Point r(*this);

  r.FlipSign();

  return r;
}

Point Point::operator+(const Point& rhs) const {
  // (-a) + (-b) = -(a + b)
  // (-a) + b = -(a - b)
  // a + b = a + b
  // a + (-b) = a - b
  if (is_negative_ && rhs.is_negative_)
    return -AddPositive(-*this, -rhs);
  else if (is_negative_ && !rhs.is_negative_)
    return -SubstractPositive(-*this, rhs);
  else if (!is_negative_ && !rhs.Negative())
    return AddPositive(*this, rhs);
  else  // (!_negative && other._negative
    return SubstractPositive(*this, -rhs);
}

Point Point::operator-(const Point& rhs) const {
  // (-a) - (-b) = (-a) + b = -(a - b)
  // (-a) - b = -(a + b)
  // a - b = a - b
  // a - (-a) = a + b
  if (is_negative_ && rhs.is_negative_)
    return -SubstractPositive(-*this, -rhs);
  else if (is_negative_ && !rhs.is_negative_)
    return -AddPositive(-*this, rhs);
  else if (!is_negative_ && !rhs.Negative())
    return SubstractPositive(*this, rhs);
  else  // (!_negative && other._negative
    return AddPositive(*this, -rhs);
}

inline Point Point::operator*(double factor) const {
  Point ret = *this;
  ret *= factor;
  return ret;
}

inline Point operator*(double lhs, const Point& rhs_) { return rhs_ * lhs; }

Point Point::AddPositive(const Point& a, const Point& b) const {
  assert(!a.is_negative_);
  assert(!b.is_negative_);

  Point r;

  r.s_ = a.s_ + b.s_;
  r.us_ = a.us_ + b.us_;

  r.s_ += (r.us_ / 1000000);
  r.us_ %= 1000000;

  r.FixZeroSign();
  return r;
}

Point Point::SubstractPositive(const Point& a, const Point& b) const {
  assert(!a.is_negative_);
  assert(!b.is_negative_);

  Point r;

  if (a == b) return r;

  r.us_ = a.us_ - b.us_;

  if (a > b) {
    r.is_negative_ = false;
    r.s_ = a.s_ - b.s_;

    if (r.us_ < 0) {
      r.us_ = 1000000 + r.us_;
      r.s_--;
    }
  } else  // (a<b)
  {
    r.is_negative_ = true;
    r.s_ = b.s_ - a.s_;

    if (r.us_ < 0)
      r.us_ *= -1;
    else if (r.us_ > 0) {
      r.s_--;
      r.us_ = 1000000 - r.us_;
    }
  }

  return r;
}

bool Point::operator==(const Point& rhs) const {
  return ((is_negative_ == rhs.is_negative_) && (s_ == rhs.s_) &&
          (us_ == rhs.us_));
}

bool Point::operator!=(const Point& rhs) const { return !(*this == rhs); }

bool Point::operator>(const Point& rhs) const {
  if (is_negative_ && !rhs.is_negative_)
    return false;
  else if (!is_negative_ && rhs.is_negative_)
    return true;
  else if (is_negative_ && rhs.is_negative_) {
    if (s_ < rhs.s_)
      return true;
    else if (s_ > rhs.s_)
      return false;
    else  // (_s == rhs._s)
    {
      if (us_ < rhs.us_)
        return true;
      else if (us_ > rhs.us_)
        return false;
      else  // (_us==rhs._us)
        return false;
    }
  } else  // (!_negative && !rhs._negative)
  {
    if (s_ < rhs.s_)
      return false;
    else if (s_ > rhs.s_)
      return true;
    else  // (_s == _other._s)
    {
      if (us_ < rhs.us_)
        return false;
      else if (us_ > rhs.us_)
        return true;
      else  // (_us==rhs._us)
        return false;
    }
  }
}

bool Point::operator>=(const Point& rhs) const {
  return ((*this == rhs) || (*this > rhs));
}

bool Point::operator<(const Point& rhs) const {
  if (is_negative_ && !rhs.is_negative_)
    return true;
  else if (!is_negative_ && rhs.is_negative_)
    return false;
  else if (is_negative_ && rhs.is_negative_) {
    if (s_ < rhs.s_)
      return false;
    else if (s_ > rhs.s_)
      return true;
    else  // (_s == _other._s)
    {
      if (us_ < rhs.us_)
        return false;
      else if (us_ > rhs.us_)
        return true;
      else  // (_us==rhs._us)
        return false;
    }
  } else  // (!_negative && !rhs._negative)
  {
    if (s_ < rhs.s_)
      return true;
    else if (s_ > rhs.s_)
      return false;
    else  // (_s == rhs._s)
    {
      if (us_ < rhs.us_)
        return true;
      else if (us_ > rhs.us_)
        return false;
      else  // (_us==rhs._us)
        return false;
    }
  }
}

bool Point::operator<=(const Point& rhs) const {
  return ((*this == rhs) || (*this < rhs));
}

Point Point::operator+=(const Point& rhs) {
  return (*this = *this + rhs);
}

Point Point::operator-=(const Point& rhs) {
  return (*this = *this - rhs);
}

Point Point::operator*=(double factor) {
  double tmp = (1000000. * s_ + us_) * factor;

  s_ = static_cast<uint32_t>(tmp / 1000000.);
  us_ = static_cast<int32_t>(tmp) % 1000000;

  this->FixZeroSign();
  return *this;
}

double Point::operator/(const Point& rhs) const {
  if (rhs == Zero()) return std::numeric_limits<double>::infinity();

  double dthis = (1000000. * s_ + us_);
  double dother = (1000000. * rhs.s_ + rhs.us_);

  return dthis / dother;
}

}  // namespace Time
}  // namespace CxxMidi

#endif  // CXXMIDI_TIME_POINT_HPP
