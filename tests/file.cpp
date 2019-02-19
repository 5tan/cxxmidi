#include <gtest/gtest.h>

#include <cxxmidi/file.hpp>
#include <cxxmidi/note.hpp>

class FileTestFixture : public ::testing::Test {
 protected:
  cxxmidi::File CreateTestFile() {
    const uint32_t dt = cxxmidi::converters::Us2dt(
        100000,  // 0.1s
        500000,  // default tempo [us/quarternote]
        500);    // default time division [us/quarternote]

    cxxmidi::File file;

    // create 3 tracks
    for (int t = 0; t < 3; t++) {
      cxxmidi::Track& track = file.AddTrack();

      for (int i = 0; i < 10; i++) {
        track.push_back(
            cxxmidi::Event(0,                             // deltatime
                           cxxmidi::Message::kNoteOn,     // message type
                           cxxmidi::Note::MiddleC() + i,  // note
                           100 + i));                     // velocity [0...127]
        track.push_back(
            cxxmidi::Event(dt,                            // deltatime
                           cxxmidi::Message::kNoteOn,     // message type
                           cxxmidi::Note::MiddleC() + i,  // note
                           0));  // velocity=0 => note off
      }
      track.push_back(cxxmidi::Event(0,  // deltatime
                                     cxxmidi::Message::kMeta,
                                     cxxmidi::Message::EndOfTrack));
    }

    return file;
  }
};

TEST_F(FileTestFixture, Creation) {
  cxxmidi::File file = CreateTestFile();
  EXPECT_EQ(file.Duration().count(), 1000000);
  EXPECT_EQ(file.Tracks(), static_cast<size_t>(3));
}
