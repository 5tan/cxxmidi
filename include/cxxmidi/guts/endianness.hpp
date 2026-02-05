/* *****************************************************************************
Copyright (c) 2018 5tan

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

#ifndef INCLUDE_CXXMIDI_GUTS_ENDIANNESS_HPP_
#define INCLUDE_CXXMIDI_GUTS_ENDIANNESS_HPP_

#include <cstdint>
#include <fstream>

namespace cxxmidi {
namespace guts {
namespace endianness {

#ifdef CXXMIDI_LIL_ENDIAN
constexpr bool MachineIsLittleEndian() { return true; }
#elif CXXMIDI_BIG_ENDIAN
constexpr bool MachineIsLittleEndian() { return false; }
#else
// We could use std::endian from <bit> in C++20, but currently it doesn't pass
// clazy check
inline bool MachineIsLittleEndian() {
  uint32_t num = 1;
  static const bool r = *reinterpret_cast<uint8_t*>(&num) == 1;
  return r;
}
#endif

// We could use std::byteswap from <bit> in C++23, but currently it doesn't pass
// clazy check
template <typename T>
T Swap(const T& u);

#ifdef __GNUC__
template <>
inline uint16_t Swap(const uint16_t& u) {
  return __builtin_bswap16(u);
}

template <>
inline uint32_t Swap(const uint32_t& u) {
  return __builtin_bswap32(u);
}
#else
template <>
inline uint16_t Swap(const uint16_t& u) {
  return (u << 8 | u >> 8);
}

template <>
inline uint32_t Swap(const uint32_t& u) {
  return ((u & 0x000000FF) << 24) | ((u & 0x0000FF00) << 8) |
         ((u & 0x00FF0000) >> 8) | ((u & 0xFF000000) >> 24);
}
#endif  // __GNUC__

// Read big-endian value from file
template <typename T>
T ReadBe(std::ifstream& file) {
  static_assert(std::is_integral_v<T>, "ReadBe only supports integral types");
  static_assert(sizeof(T) > 1, "ReadBe requires multi-byte types");
  T r;
  file.read(reinterpret_cast<char*>(&r), sizeof(T));
  if (MachineIsLittleEndian()) {
    r = Swap(r);
  }
  return r;
}

// Write big-endian value to file
template <typename T>
size_t WriteBe(std::ofstream& file, T val) {
  static_assert(std::is_integral_v<T>, "WriteBe only supports integral types");
  static_assert(sizeof(T) > 1, "WriteBe requires multi-byte types");
  if (MachineIsLittleEndian()) {
    val = Swap(val);
  }
  file.write(reinterpret_cast<char*>(&val), sizeof(val));
  return sizeof(val);
}

}  // namespace endianness
}  // namespace guts
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_GUTS_ENDIANNESS_HPP_
