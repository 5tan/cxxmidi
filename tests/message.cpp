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

#include <gtest/gtest.h>

#include <cxxmidi/message.hpp>

TEST(Message, IsVoiceCategory) {
  using cxxmidi::Message;

  EXPECT_FALSE(Message(Message::kSysExBegin).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kMtcQuarterFrame).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kSongPositionPointer).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kSongSelect).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kTuneRequest).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kSysExEnd).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kClock).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kTick).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kStart).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kContinue).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kStop).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kActiveSense).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kReset).IsVoiceCategory());
  EXPECT_FALSE(Message(Message::kMeta).IsVoiceCategory());

  for (uint8_t ch = 0; ch < 16; ch++) {
    EXPECT_TRUE(Message(ch | Message::kNoteOff).IsVoiceCategory());
    EXPECT_TRUE(Message(ch | Message::kNoteOn).IsVoiceCategory());
    EXPECT_TRUE(Message(ch | Message::kNoteAftertouch).IsVoiceCategory());
    EXPECT_TRUE(Message(ch | Message::kControlChange).IsVoiceCategory());
    EXPECT_TRUE(Message(ch | Message::kProgramChange).IsVoiceCategory());
    EXPECT_TRUE(Message(ch | Message::kChannelAftertouch).IsVoiceCategory());
    EXPECT_TRUE(Message(ch | Message::kPitchWheel).IsVoiceCategory());
    // clang-format off
    EXPECT_TRUE(Message(ch | Message::kNoteOff)
      .IsVoiceCategory(Message::kNoteOff));
    EXPECT_TRUE(Message(ch | Message::kNoteOn)
      .IsVoiceCategory(Message::kNoteOn));
    EXPECT_TRUE(Message(ch | Message::kNoteAftertouch)
      .IsVoiceCategory(Message::kNoteAftertouch));
    EXPECT_TRUE(Message(ch | Message::kControlChange)
      .IsVoiceCategory(Message::kControlChange));
    EXPECT_TRUE(Message(ch | Message::kProgramChange)
      .IsVoiceCategory(Message::kProgramChange));
    EXPECT_TRUE(Message(ch | Message::kChannelAftertouch)
      .IsVoiceCategory(Message::kChannelAftertouch));
    EXPECT_TRUE(Message(ch | Message::kPitchWheel)
      .IsVoiceCategory(Message::kPitchWheel));

    EXPECT_FALSE(Message(ch | Message::kNoteOff).
      IsVoiceCategory(Message::kNoteOn));
    // clang-format on
  }
}
