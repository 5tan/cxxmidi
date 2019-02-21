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

#ifndef INCLUDE_CXXMIDI_GUTS_UTILS_HPP_
#define INCLUDE_CXXMIDI_GUTS_UTILS_HPP_

#include <cstdint>
#include <cxxmidi/guts/endianness.hpp>

namespace cxxmidi {
namespace utils {

inline uint32_t ExtractTempo(uint8_t v0, uint8_t v1, uint8_t v2) {
  union {
    uint32_t one_tempo;
    uint8_t tab_tempo[3];
  };
  one_tempo = 0;

  if (guts::endianness::MachineIsLittleEndian()) {
    tab_tempo[0] = v2;
    tab_tempo[1] = v1;
    tab_tempo[2] = v0;
  } else {
    tab_tempo[0] = v0;
    tab_tempo[1] = v1;
    tab_tempo[2] = v2;
  }

  return one_tempo;
}

inline uint32_t GetVlq(std::ifstream &file) {
  uint32_t r = 0;
  uint8_t c;

  do {
    file.read(reinterpret_cast<char *>(&c), 1);
    r = (r << 7) + (c & 0x7f);
  } while (c & 0x80);

  return r;
}

inline size_t SaveVlq(std::ofstream &output_file, unsigned int val) {
  size_t r = 0;
  uint32_t vlq = val & 0x7f;

  // prepare variable-length quantity
  while ((val >>= 7) > 0) {
    vlq <<= 8;
    vlq |= 0x80;
    vlq += (val & 0x7f);
  }

  // save variable-length quantity
  while (true) {
    r++;
    output_file.write(reinterpret_cast<char *>(&vlq), 1);
    if (vlq & 0x80)
      vlq >>= 8;
    else
      break;
  }

  return r;  // return size of vlq
}

}  // namespace utils
}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_GUTS_UTILS_HPP_
