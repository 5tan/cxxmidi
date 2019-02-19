#include <gtest/gtest.h>
#include <cxxmidi/guts/endianness.hpp>

TEST(Endianness, Fix) {
  std::ifstream ifs("../../cxxmidi/music/chopin.mid");
  uint32_t data;
  ifs.read(reinterpret_cast<char *>(&data), 4);

  if (cxxmidi::guts::endianness::MachineIsLittleEndian()) {
    ASSERT_EQ(data, 0x6468544d);  // "dhTM" (fix needed)
    ASSERT_EQ(cxxmidi::guts::endianness::Fix(data), 0x4d546864);  // "MThd"
  } else {
    ASSERT_EQ(data, 0x4d546864);  // "MThd" (no fix needed)
    ASSERT_EQ(cxxmidi::guts::endianness::Fix(data), 0x6468544d);  // "dhTM"
  }
}
