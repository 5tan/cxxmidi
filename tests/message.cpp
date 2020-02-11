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

#include <gtest/gtest.h>

#include <cxxmidi/message.hpp>

TEST(Message, IsRealtime) {
  using cxxmidi::Message;

  for (uint8_t ch = 0; ch < 16; ch++) {
    EXPECT_FALSE(Message(ch | Message::kNoteOff).IsRealtime());
    EXPECT_FALSE(Message(ch | Message::kNoteOn).IsRealtime());
    EXPECT_FALSE(Message(ch | Message::kNoteAftertouch).IsRealtime());
    EXPECT_FALSE(Message(ch | Message::kControlChange).IsRealtime());
    EXPECT_FALSE(Message(ch | Message::kProgramChange).IsRealtime());
    EXPECT_FALSE(Message(ch | Message::kChannelAftertouch).IsRealtime());
    EXPECT_FALSE(Message(ch | Message::kPitchWheel).IsRealtime());
  }

  EXPECT_TRUE(Message(Message::kClock).IsRealtime());
  EXPECT_TRUE(Message(Message::kTick).IsRealtime());
  EXPECT_TRUE(Message(Message::kStart).IsRealtime());
  EXPECT_TRUE(Message(Message::kContinue).IsRealtime());
  EXPECT_TRUE(Message(Message::kStop).IsRealtime());
  EXPECT_TRUE(Message(Message::kActiveSense).IsRealtime());
  EXPECT_TRUE(Message(Message::kReset).IsRealtime());

  EXPECT_FALSE(Message(Message::kSysExBegin).IsRealtime());
  EXPECT_FALSE(Message(Message::kMtcQuarterFrame).IsRealtime());
  EXPECT_FALSE(Message(Message::kSongPositionPointer).IsRealtime());
  EXPECT_FALSE(Message(Message::kSongSelect).IsRealtime());
  EXPECT_FALSE(Message(Message::kTuneRequest).IsRealtime());
  EXPECT_FALSE(Message(Message::kSysExEnd).IsRealtime());

  // EXPECT_FALSE(Message(Message::kMeta).IsRealtime()); // kMeta == kReset
}

TEST(Message, IsSystemCommon) {
  using cxxmidi::Message;

  for (uint8_t ch = 0; ch < 16; ch++) {
    EXPECT_FALSE(Message(ch | Message::kNoteOff).IsSystemCommon());
    EXPECT_FALSE(Message(ch | Message::kNoteOn).IsSystemCommon());
    EXPECT_FALSE(Message(ch | Message::kNoteAftertouch).IsSystemCommon());
    EXPECT_FALSE(Message(ch | Message::kControlChange).IsSystemCommon());
    EXPECT_FALSE(Message(ch | Message::kProgramChange).IsSystemCommon());
    EXPECT_FALSE(Message(ch | Message::kChannelAftertouch).IsSystemCommon());
    EXPECT_FALSE(Message(ch | Message::kPitchWheel).IsSystemCommon());
  }

  EXPECT_FALSE(Message(Message::kClock).IsSystemCommon());
  EXPECT_FALSE(Message(Message::kTick).IsSystemCommon());
  EXPECT_FALSE(Message(Message::kStart).IsSystemCommon());
  EXPECT_FALSE(Message(Message::kContinue).IsSystemCommon());
  EXPECT_FALSE(Message(Message::kStop).IsSystemCommon());
  EXPECT_FALSE(Message(Message::kActiveSense).IsSystemCommon());
  EXPECT_FALSE(Message(Message::kReset).IsSystemCommon());

  EXPECT_TRUE(Message(Message::kSysExBegin).IsSystemCommon());
  EXPECT_TRUE(Message(Message::kMtcQuarterFrame).IsSystemCommon());
  EXPECT_TRUE(Message(Message::kSongPositionPointer).IsSystemCommon());
  EXPECT_TRUE(Message(Message::kSongSelect).IsSystemCommon());
  EXPECT_TRUE(Message(Message::kTuneRequest).IsSystemCommon());
  EXPECT_TRUE(Message(Message::kSysExEnd).IsSystemCommon());

  EXPECT_FALSE(Message(Message::kMeta).IsSystemCommon());
}

TEST(Message, IsVoiceCategory) {
  using Msg = cxxmidi::Message;

  for (uint8_t ch = 0; ch < 16; ch++) {
    EXPECT_TRUE(Msg(ch | Msg::kNoteOff).IsVoiceCategory());
    EXPECT_TRUE(Msg(ch | Msg::kNoteOn).IsVoiceCategory());
    EXPECT_TRUE(Msg(ch | Msg::kNoteAftertouch).IsVoiceCategory());
    EXPECT_TRUE(Msg(ch | Msg::kControlChange).IsVoiceCategory());
    EXPECT_TRUE(Msg(ch | Msg::kProgramChange).IsVoiceCategory());
    EXPECT_TRUE(Msg(ch | Msg::kChannelAftertouch).IsVoiceCategory());
    EXPECT_TRUE(Msg(ch | Msg::kPitchWheel).IsVoiceCategory());

    EXPECT_TRUE(Msg(ch | Msg::kNoteOff).IsVoiceCategory(Msg::kNoteOff));
    EXPECT_TRUE(Msg(ch | Msg::kNoteOn).IsVoiceCategory(Msg::kNoteOn));
    EXPECT_TRUE(
        Msg(ch | Msg::kNoteAftertouch).IsVoiceCategory(Msg::kNoteAftertouch));
    EXPECT_TRUE(
        Msg(ch | Msg::kControlChange).IsVoiceCategory(Msg::kControlChange));
    EXPECT_TRUE(
        Msg(ch | Msg::kProgramChange).IsVoiceCategory(Msg::kProgramChange));
    EXPECT_TRUE(Msg(ch | Msg::kChannelAftertouch)
                    .IsVoiceCategory(Msg::kChannelAftertouch));
    EXPECT_TRUE(Msg(ch | Msg::kPitchWheel).IsVoiceCategory(Msg::kPitchWheel));

    EXPECT_FALSE(Msg(ch | Msg::kNoteOff).IsVoiceCategory(Msg::kNoteOn));
  }

  EXPECT_FALSE(Msg(Msg::kSysExBegin).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kMtcQuarterFrame).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kSongPositionPointer).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kSongSelect).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kTuneRequest).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kSysExEnd).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kClock).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kTick).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kStart).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kContinue).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kStop).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kActiveSense).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kReset).IsVoiceCategory());
  EXPECT_FALSE(Msg(Msg::kMeta).IsVoiceCategory());
}

TEST(Message, GetName) {
  using Msg = cxxmidi::Message;

  for (uint8_t ch = 0; ch < 16; ch++) {
    EXPECT_EQ(Msg(ch | Msg::kNoteOff).GetName(), "NoteOff");
    EXPECT_EQ(Msg(ch | Msg::kNoteOn).GetName(), "NoteOn");
    EXPECT_EQ(Msg(ch | Msg::kNoteAftertouch).GetName(), "NoteAftertouch");
    EXPECT_EQ(Msg(ch | Msg::kControlChange).GetName(), "ControlChange");
    EXPECT_EQ(Msg(ch | Msg::kProgramChange).GetName(), "ProgramChange");
    EXPECT_EQ(Msg(ch | Msg::kChannelAftertouch).GetName(), "ChannelAftertouch");
    EXPECT_EQ(Msg(ch | Msg::kPitchWheel).GetName(), "PitchWheel");
  }

  EXPECT_EQ(Msg(Msg::kSysExBegin).GetName(), "SysExBegin");
  EXPECT_EQ(Msg(Msg::kMtcQuarterFrame).GetName(), "MtcQuarterFrame");
  EXPECT_EQ(Msg(Msg::kSongPositionPointer).GetName(), "SongPositionPointer");
  EXPECT_EQ(Msg(Msg::kSongSelect).GetName(), "SongSelect");
  EXPECT_EQ(Msg(Msg::kTuneRequest).GetName(), "TuneRequest");
  EXPECT_EQ(Msg(Msg::kSysExEnd).GetName(), "SysExEnd");

  EXPECT_EQ(Msg(Msg::kClock).GetName(), "Clock");
  EXPECT_EQ(Msg(Msg::kTick).GetName(), "Tick");
  EXPECT_EQ(Msg(Msg::kStart).GetName(), "Start");
  EXPECT_EQ(Msg(Msg::kContinue).GetName(), "Continue");
  EXPECT_EQ(Msg(Msg::kStop).GetName(), "Stop");
  EXPECT_EQ(Msg(Msg::kActiveSense).GetName(), "ActiveSense");
  EXPECT_EQ(Msg(Msg::kReset).GetName(), "Reset");

  EXPECT_EQ(Msg(Msg::kMeta, Msg::kSequenceNumber).GetName(), "SequenceNumber");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kText).GetName(), "Text");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kCopyright).GetName(), "Copyright");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kTrackName).GetName(), "TrackName");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kInstrumentName).GetName(), "InstrumentName");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kLyrics).GetName(), "Lyrics");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kMarker).GetName(), "Marker");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kCuePoint).GetName(), "CuePoint");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kChannelPrefix).GetName(), "ChannelPrefix");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kOutputCable).GetName(), "OutputCable");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kEndOfTrack).GetName(), "EndOfTrack");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kTempo).GetName(), "Tempo");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kSmpteOffset).GetName(), "SmpteOffset");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kTimeSignature).GetName(), "TimeSignature");
  EXPECT_EQ(Msg(Msg::kMeta, Msg::kKeySignature).GetName(), "KeySignature");
}

TEST(Message, GetType) {
  using cxxmidi::Message;
  Message msg(Message::kNoteOn);
  ASSERT_EQ(msg.GetType(), Message::kNoteOn);
}
