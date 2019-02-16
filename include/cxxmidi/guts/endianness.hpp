#ifndef CXXMIDI_GUTS_ENDIANNESS_HPP
#define CXXMIDI_GUTS_ENDIANNESS_HPP

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

}  // namespace Endianness
}  // namespace Guts
}  // namespace CxxMidi

#endif  // CXXMIDI_GUTS_ENDIANNESS_HPP
