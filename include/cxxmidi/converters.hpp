#ifndef CXXMIDI_CONVERTERS_HPP
#define CXXMIDI_CONVERTERS_HPP

#include <cxxmidi/utils.hpp>

namespace cxxmidi {
namespace converters {

inline constexpr unsigned int Dt2us(uint32_t dt, unsigned int tempo_uspq,
                                    uint16_t time_div) {
  return dt * utils::UsPerTick(tempo_uspq, time_div);
}

inline constexpr uint32_t Us2dt(unsigned int us, unsigned int tempo_uspq,
                                uint16_t time_div) {
  return us / utils::UsPerTick(tempo_uspq, time_div);
}

}  // namespace Converters
}  // namespace CxxMidi

#endif  // CXXMIDI_CONVERTERS_HPP
