#include <gtest/gtest.h>
#include <cxxmidi/instrument.hpp>

TEST (Instrument, GetSomeNames) {
    EXPECT_STREQ(CxxMidi::Instrument::name(0).c_str(), "Acoustic Grand Piano");
    EXPECT_STREQ(CxxMidi::Instrument::name(127).c_str(), "Gunshot");
}

TEST (Instrument, GetSomeIds) {
    EXPECT_EQ(static_cast<int>(CxxMidi::Instrument(CxxMidi::Instrument::AcousticGrandPiano)), 0);
    EXPECT_EQ(static_cast<int>(CxxMidi::Instrument(CxxMidi::Instrument::Gunshot)), 127);
}
