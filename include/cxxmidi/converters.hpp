#ifndef CXXMIDI_CONVERTERS_HPP
#define CXXMIDI_CONVERTERS_HPP

#include <cxxmidi/utils.hpp>

namespace CxxMidi {
namespace Converters {

inline unsigned int dt2us(uint32_t dt_, unsigned int tempo_uspq_, uint16_t timeDiv_)
{
    return dt_*Utils::usPerTick(tempo_uspq_,timeDiv_);
}

inline uint32_t us2dt(unsigned int us_, unsigned int tempo_uspq_, uint16_t timeDiv_)
{
    return us_/Utils::usPerTick(tempo_uspq_,timeDiv_);
}

} // namespace Converters
} // namespace CxxMidi

#endif // CXXMIDI_CONVERTERS_HPP
