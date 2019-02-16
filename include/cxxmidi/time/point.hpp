#ifndef CXXMIDI_TIME_POINT_HPP
#define CXXMIDI_TIME_POINT_HPP

#include <cstdint>

namespace cxxmidi {
namespace time {

class Point {
 public:
  inline Point();
  inline static Point FromUs(int us_);
  inline static Point FromMs(int ms_);
  inline static Point FromS(int s_);
  inline static Point FromTimecode(const std::string& tc_);
  inline static Point Zero();

  inline std::string ToTimecode(bool includeUs_ = false) const;

  inline bool Negative() const { return _negative; }
  inline void SetNegative(bool negative_);
  inline void FlipSign();
  inline unsigned int S() { return _s; }
  inline unsigned int Us() { return _us; }

  inline void AddUs(int us_);
  inline void AddMs(int ms_);
  inline void AddS(int s_);

  inline Point operator-() const;
  inline Point operator+(const Point& other_) const;
  inline Point operator-(const Point& other_) const;
  inline Point operator*(double factor_) const;
  inline Point operator+=(const Point& other_);
  inline Point operator-=(const Point& other_);
  inline Point operator*=(double factor_);
  inline bool operator==(const Point& other_) const;
  inline bool operator!=(const Point& other_) const;
  inline bool operator>(const Point& other_) const;
  inline bool operator>=(const Point& other_) const;
  inline bool operator<(const Point& other_) const;
  inline bool operator<=(const Point& other_) const;
  inline double operator/(const Point& other_) const;

 private:
  bool _negative;
  uint32_t _s;
  int32_t _us;

  inline Point AddPositive(const Point& a, const Point& b) const;
  inline Point SubstractPositive(const Point& a, const Point& b) const;
  inline void FixZeroSign();
};

inline Point operator*(double lhs_, const Point& rhs_);

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

Point::Point() : _negative(false), _s(0), _us(0) {}

std::string Point::ToTimecode(bool includeUs_) const {
  std::stringstream ss;

  ss << (_negative ? "-" : "") << std::setw(2) << std::setfill('0')
     << (_s / (60 * 60)) << ":" << std::setw(2) << std::setfill('0')
     << ((_s / 60) % 60) << ":" << std::setw(2) << std::setfill('0')
     << (_s % 60);

  if (includeUs_ && _us) ss << "&" << _us;

  return ss.str();
}

Point Point::Zero() { return Point(); }

Point Point::FromTimecode(const std::string& tc_) {
  Point r;
  int hh, mm, ss, us = 0;

  if (tc_.find('&') == std::string::npos)
    std::sscanf(tc_.c_str(), "%d:%d:%d", &hh, &mm, &ss);
  else {
    std::sscanf(tc_.c_str(), "%d:%d:%d&%d", &hh, &mm, &ss, &us);
    r._us = us;
  }

  r._s = ss + (60 * mm) + (60 * 60 * hh);
  r._s += us / 1000000;
  r._us %= 1000000;

  return r;
}

Point Point::FromUs(int us_) {
  Point r;

  if (us_ < 0) {
    r._negative = true;
    us_ = std::abs(us_);
  }

  r._s = us_ / 1000000;
  r._us = us_ % 1000000;

  r.FixZeroSign();
  return r;
}

Point Point::FromMs(int ms_) {
  Point r;

  if (ms_ < 0) {
    r._negative = true;
    ms_ = std::abs(ms_);
  }

  r._s = ms_ / 1000;
  r._us = (ms_ % 1000) * 1000;

  r.FixZeroSign();
  return r;
}

Point Point::FromS(int s_) {
  Point r;

  if (s_ < 0) {
    r._negative = true;
    s_ = std::abs(s_);
  }

  r._s = s_;

  r.FixZeroSign();
  return r;
}

void Point::AddUs(int us_) {
  *this = *this + FromUs(us_);
  this->FixZeroSign();
}

void Point::AddMs(int ms_) {
  *this = *this + FromMs(ms_);
  this->FixZeroSign();
}

void Point::AddS(int s_) {
  *this = *this + FromS(s_);
  this->FixZeroSign();
}

void Point::FixZeroSign() {
  if (!_s)
    if (!_us) _negative = false;
}

void Point::FlipSign() {
  _negative = !_negative;
  this->FixZeroSign();
}

void Point::SetNegative(bool negative_) {
  _negative = negative_;
  this->FixZeroSign();
}

Point Point::operator-() const {
  Point r(*this);

  r.FlipSign();

  return r;
}

Point Point::operator+(const Point& other_) const {
  // (-a) + (-b) = -(a + b)
  // (-a) + b = -(a - b)
  // a + b = a + b
  // a + (-b) = a - b
  if (_negative && other_._negative)
    return -AddPositive(-*this, -other_);
  else if (_negative && !other_._negative)
    return -SubstractPositive(-*this, other_);
  else if (!_negative && !other_.Negative())
    return AddPositive(*this, other_);
  else  // (!_negative && other._negative
    return SubstractPositive(*this, -other_);
}

Point Point::operator-(const Point& other_) const {
  // (-a) - (-b) = (-a) + b = -(a - b)
  // (-a) - b = -(a + b)
  // a - b = a - b
  // a - (-a) = a + b
  if (_negative && other_._negative)
    return -SubstractPositive(-*this, -other_);
  else if (_negative && !other_._negative)
    return -AddPositive(-*this, other_);
  else if (!_negative && !other_.Negative())
    return SubstractPositive(*this, other_);
  else  // (!_negative && other._negative
    return AddPositive(*this, -other_);
}

inline Point Point::operator*(double factor_) const {
  Point ret = *this;
  ret *= factor_;
  return ret;
}

inline Point operator*(double lhs_, const Point& rhs_) { return rhs_ * lhs_; }

Point Point::AddPositive(const Point& a, const Point& b) const {
  assert(!a._negative);
  assert(!b._negative);

  Point r;

  r._s = a._s + b._s;
  r._us = a._us + b._us;

  r._s += (r._us / 1000000);
  r._us %= 1000000;

  r.FixZeroSign();
  return r;
}

Point Point::SubstractPositive(const Point& a, const Point& b) const {
  assert(!a._negative);
  assert(!b._negative);

  Point r;

  if (a == b) return r;

  r._us = a._us - b._us;

  if (a > b) {
    r._negative = false;
    r._s = a._s - b._s;

    if (r._us < 0) {
      r._us = 1000000 + r._us;
      r._s--;
    }
  } else  // (a<b)
  {
    r._negative = true;
    r._s = b._s - a._s;

    if (r._us < 0)
      r._us *= -1;
    else if (r._us > 0) {
      r._s--;
      r._us = 1000000 - r._us;
    }
  }

  return r;
}

bool Point::operator==(const Point& other_) const {
  return ((_negative == other_._negative) && (_s == other_._s) &&
          (_us == other_._us));
}

bool Point::operator!=(const Point& other_) const { return !(*this == other_); }

bool Point::operator>(const Point& other_) const {
  if (_negative && !other_._negative)
    return false;
  else if (!_negative && other_._negative)
    return true;
  else if (_negative && other_._negative) {
    if (_s < other_._s)
      return true;
    else if (_s > other_._s)
      return false;
    else  // (_s == other_._s)
    {
      if (_us < other_._us)
        return true;
      else if (_us > other_._us)
        return false;
      else  // (_us==other_._us)
        return false;
    }
  } else  // (!_negative && !other_._negative)
  {
    if (_s < other_._s)
      return false;
    else if (_s > other_._s)
      return true;
    else  // (_s == _other._s)
    {
      if (_us < other_._us)
        return false;
      else if (_us > other_._us)
        return true;
      else  // (_us==other_._us)
        return false;
    }
  }
}

bool Point::operator>=(const Point& other_) const {
  return ((*this == other_) || (*this > other_));
}

bool Point::operator<(const Point& other_) const {
  if (_negative && !other_._negative)
    return true;
  else if (!_negative && other_._negative)
    return false;
  else if (_negative && other_._negative) {
    if (_s < other_._s)
      return false;
    else if (_s > other_._s)
      return true;
    else  // (_s == _other._s)
    {
      if (_us < other_._us)
        return false;
      else if (_us > other_._us)
        return true;
      else  // (_us==other_._us)
        return false;
    }
  } else  // (!_negative && !other_._negative)
  {
    if (_s < other_._s)
      return true;
    else if (_s > other_._s)
      return false;
    else  // (_s == other_._s)
    {
      if (_us < other_._us)
        return true;
      else if (_us > other_._us)
        return false;
      else  // (_us==other_._us)
        return false;
    }
  }
}

bool Point::operator<=(const Point& other_) const {
  return ((*this == other_) || (*this < other_));
}

Point Point::operator+=(const Point& other_) {
  return (*this = *this + other_);
}

Point Point::operator-=(const Point& other_) {
  return (*this = *this - other_);
}

Point Point::operator*=(double factor_) {
  double tmp = (1000000. * _s + _us) * factor_;

  _s = static_cast<uint32_t>(tmp / 1000000.);
  _us = static_cast<int32_t>(tmp) % 1000000;

  this->FixZeroSign();
  return *this;
}

double Point::operator/(const Point& other_) const {
  if (other_ == Zero()) return std::numeric_limits<double>::infinity();

  double dthis = (1000000. * _s + _us);
  double dother = (1000000. * other_._s + other_._us);

  return dthis / dother;
}

}  // namespace Time
}  // namespace CxxMidi

#endif  // CXXMIDI_TIME_POINT_HPP
