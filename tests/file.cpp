#include <gtest/gtest.h>

#include <cxxmidi/file.hpp>
#include <cxxmidi/note.hpp>

class FileTestFixture : public ::testing::Test
{
protected:

    CxxMidi::File createTestFile()
    {
        const uint32_t dt = CxxMidi::Converters::us2dt(
                    100000, // 0.1s
                    500000, // default tempo [us/quarternote]
                    500); // default time division [us/quarternote]

        CxxMidi::File file;

        // create 3 tracks
        for(int t=0; t<3; t++)
        {
            CxxMidi::Track& track = file.addTrack();

            for(int i=0; i<10; i++)
            {
                track.push_back(CxxMidi::Event(
                                    0, // deltatime
                                    CxxMidi::Message::NoteOn, // message type
                                    CxxMidi::Note::MiddleC()+i, // note
                                    100+i)); // velocity [0...127]
                track.push_back(CxxMidi::Event(
                                    dt, // deltatime
                                    CxxMidi::Message::NoteOn, // message type
                                    CxxMidi::Note::MiddleC()+i, // note
                                    0)); // velocity=0 => note off
            }
            track.push_back(CxxMidi::Event(0, // deltatime
                                           CxxMidi::Message::Meta,
                                           CxxMidi::Message::EndOfTrack));
        }

        return file;
    }
};

TEST_F(FileTestFixture, Creation)
{
        CxxMidi::File file = createTestFile();
        EXPECT_STREQ(file.duration().toTimecode().c_str(),"00:00:01");
        EXPECT_EQ(file.tracks(),static_cast<size_t>(3));
}

