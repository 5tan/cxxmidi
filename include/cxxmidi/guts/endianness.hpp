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

#ifndef INCLUDE_CXXMIDI_GUTS_ENDIANNESS_HPP_
#define INCLUDE_CXXMIDI_GUTS_ENDIANNESS_HPP_

#include <cstdint>
#include <fstream>
#include <iostream>

namespace cxxmidi {
namespace guts {
namespace endianness {

#ifdef CXXMIDI_LIL_ENDIAN
inline constexpr bool MachineIsLittleEndian() { return true; }
#elif CXXMIDI_BIG_ENDIAN
inline constexpr bool MachineIsLittleEndian() { return false; }
#else
inline bool MachineIsLittleEndian() {
  uint32_t num = 1;
  static const bool r = *reinterpret_cast<uint8_t *>(&num) == 1;
  return r;
}
#endif

template <typename T>
T Swap(const T &u) {
  union {
    T u;
    unsigned char u8[sizeof(T)];
  } source, dest;
  source.u = u;

  for (size_t k = 0; k < sizeof(T); k++)
    dest.u8[k] = source.u8[sizeof(T) - k - 1];

  return dest.u;
}

template <>
inline uint16_t Swap(const uint16_t &u) {
  return (u << 8 | u >> 8);
}

#ifdef __GNUC__
template <>
inline uint32_t Swap(const uint32_t &u) {
  return __builtin_bswap32(u);
}

template <>
inline uint64_t Swap(const uint64_t &u) {
  return __builtin_bswap64(u);
}
#endif  // __GNUC__

template <typename T>
T ReadBe(std::ifstream &file) {
  T r;
  file.read(reinterpret_cast<char *>(&r), sizeof(T));
  if ((sizeof(T) > 1) && MachineIsLittleEndian()) r = Swap<T>(r);
  return r;
}

template <typename T>
size_t WriteBe(std::ofstream &file, T val) {
  size_t size = sizeof(val);
  if ((sizeof(T) > 1) && MachineIsLittleEndian()) val = Swap<T>(val);
  file.write(reinterpret_cast<char *>(&val), size);
  return size;
}

}  // namespace endianness
}  // namespace guts
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_GUTS_ENDIANNESS_HPP_
