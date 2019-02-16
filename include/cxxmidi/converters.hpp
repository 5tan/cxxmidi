#ifndef CXXMIDI_CONVERTERS_HPP
#define CXXMIDI_CONVERTERS_HPP

#include <cxxmidi/utils.hpp>

namespace cxxmidi {
namespace converters {

inline constexpr unsigned int Dt2us(uint32_t dt_, unsigned int tempo_uspq_,
                                    uint16_t timeDiv_) {
  return dt_ * utils::UsPerTick(tempo_uspq_, timeDiv_);
}

inline constexpr uint32_t Us2dt(unsigned int us_, unsigned int tempo_uspq_,
                                uint16_t timeDiv_) {
  return us_ / utils::UsPerTick(tempo_uspq_, timeDiv_);
}

}  // namespace Converters
}  // namespace CxxMidi

#endif  // CXXMIDI_CONVERTERS_HPP
