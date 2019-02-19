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

#ifndef CXXMIDI_CONVERTERS_HPP
#define CXXMIDI_CONVERTERS_HPP

#include <chrono>
#include <cxxmidi/guts/utils.hpp>

namespace cxxmidi {
namespace converters {

inline constexpr std::chrono::microseconds Dt2us(uint32_t dt, unsigned int tempo_uspq,
                                    uint16_t time_div) {
  return std::chrono::microseconds(dt * utils::UsPerTick(tempo_uspq, time_div));
}

inline constexpr uint32_t Us2dt(unsigned int us, unsigned int tempo_uspq,
                                uint16_t time_div) {
  return us / utils::UsPerTick(tempo_uspq, time_div);
}

}  // namespace converters
}  // namespace cxxmidi

#endif  // CXXMIDI_CONVERTERS_HPP
