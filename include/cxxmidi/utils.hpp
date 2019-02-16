#ifndef CXXMIDI_GUTS_HPP
#define CXXMIDI_GUTS_HPP

#include <cstdint>
#include <cxxmidi/guts/endianness.hpp>

namespace cxxmidi {
namespace utils {

inline uint32_t ExtractTempo(uint8_t v0, uint8_t v1, uint8_t v2) {
  union {
    uint32_t oneTempo;
    uint8_t tabTempo[3];
  };
  oneTempo = 0;

  if (guts::endianness::MachineIsLittleEndian()) {
    tabTempo[0] = v2;
    tabTempo[1] = v1;
    tabTempo[2] = v0;
  } else {
    tabTempo[0] = v0;
    tabTempo[1] = v1;
    tabTempo[2] = v2;
  }

  return oneTempo;
}

inline constexpr unsigned int UsPerTick(unsigned int tempo_uspq,
                                        uint16_t time_div) {
  return tempo_uspq / time_div;
}

inline uint32_t GetVlq(std::fstream &file) {
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

}  // namespace Utils
}  // namespace CxxMidi

#endif  // CXXMIDI_GUTS_HPP
