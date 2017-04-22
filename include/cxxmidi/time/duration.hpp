#ifndef CXXMIDI_TIME_DURATION_HPP
#define CXXMIDI_TIME_DURATION_HPP

#include <cxxmidi/time/point.hpp>

namespace CxxMidi {
namespace Time {

class Period;

class Duration
{
public:
    inline Duration();
    inline Duration(const Period& period_);
    inline Duration(const Point& p1_, const Point& p2_);

    inline void addS(int s_);
    inline void addMs(int ms_);
    inline void addUs(int us_);

    inline Point toPoint() const;
    inline std::string toTimecode(bool includeUs_=false) const;

private:
    Point _point;
};

} // namespace Time
} // namespace CxxMidi

#include <ostream>

#include <cxxmidi/time/period.hpp>

inline std::ostream& operator<<(std::ostream& os_,
                                const CxxMidi::Time::Duration& td_)
{
    os_ << td_.toTimecode(true);
    return os_;
}

namespace CxxMidi {
namespace Time {

Duration::Duration()
{
}

Duration::Duration(const Period& period_)
{
    _point = period_.first - period_.second;
    if(_point.negative())
        _point.flipSign();
}

Duration::Duration(const Point& p1_, const Point& p2_)
{
    _point = p1_-p2_;
    if(_point.negative())
        _point.flipSign();
}

Point Duration::toPoint() const
{
    return _point;
}

std::string Duration::toTimecode(bool includeUs_) const
{
    return _point.toTimecode(includeUs_);
}

void Duration::addS(int s_)
{
    _point += Point::fromS(s_);
}

void Duration::addMs(int ms_)
{
    _point += Point::fromMs(ms_);
}

void Duration::addUs(int us_)
{
    _point += Point::fromUs(us_);
}

} // namespace Time
} // namespace CxxMidi

#endif // CXXMIDI_TIME_DURATION_HPP
