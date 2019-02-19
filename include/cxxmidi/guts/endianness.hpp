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
#include <iostream>

namespace cxxmidi {
namespace guts {
namespace endianness {

inline bool MachineIsLittleEndian() {
  static const uint32_t num = 1;
  static const bool r = *(uint8_t *)&num == 1;
  return r;
  // note: we don't want constexpr here. Library should detect endianness
  // in runtime so cross compilation is possible.
}

template <typename T>
T Fix(const T &v) {
  T r = 0;

  for (size_t i = 0; i < sizeof(v); i++) {
    const T size = static_cast<T>(sizeof(v));
    const T it = static_cast<T>(i);

    T mask1 = 0xff << 8 * i;
    T byte = (v & mask1) >> 8 * i;
    T offset = (size - it - 1) * 8;
    T mask2 = 0xff << offset;
    r |= (((byte) << offset) & mask2);
  }

  return r;
}

template <typename T>
T ReadBe(std::fstream &file) {
  T r;
  file.read(reinterpret_cast<char *>(&r), sizeof(T));

  if (MachineIsLittleEndian()) r = Fix<T>(r);

  return r;
}

template <typename T>
size_t WriteBe(std::ofstream &file, T val) {
  size_t size = sizeof(val);

  if (MachineIsLittleEndian()) val = Fix<T>(val);

  file.write(reinterpret_cast<char *>(&val), size);

#ifndef CXXMIDI_QUIET
  if (file.bad()) std::cerr << "CxxMidi: file write error" << std::endl;
#endif

  return size;
}

}  // namespace endianness
}  // namespace guts
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_GUTS_ENDIANNESS_HPP_
