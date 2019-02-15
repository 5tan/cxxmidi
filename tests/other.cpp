#include <gtest/gtest.h>

#include <cxxmidi/event.hpp>
#include <cxxmidi/message.hpp>
#include <cxxmidi/track.hpp>

//CxxMidi::File Tests::buildTestFile()
//{
//    const uint32_t dt = CxxMidi::Converters::us2dt(
//                100000, // 0.1s
//                500000, // default tempo [us/quarternote]
//                500); // default time division [us/quarternote]

//    CxxMidi::File file;

//    // create 3 tracks
//    for(int t=0; t<3; t++)
//    {
//        CxxMidi::Track& track = file.addTrack();

//        for(int i=0; i<10; i++)
//        {
//            track.push_back(CxxMidi::Event(
//                                0, // deltatime
//                                CxxMidi::Message::NoteOn, // message type
//                                CxxMidi::Note::MiddleC()+i, // note
//                                100+i)); // velocity [0...127]
//            track.push_back(CxxMidi::Event(
//                                dt, // deltatime
//                                CxxMidi::Message::NoteOn, // message type
//                                CxxMidi::Note::MiddleC()+i, // note
//                                0)); // velocity=0 => note off
//        }
//        track.push_back(CxxMidi::Event(0, // deltatime
//                                       CxxMidi::Message::Meta,
//                                       CxxMidi::Message::EndOfTrack));
//    }

//    return file;
//}


//void Tests::file()
//{
//    CxxMidi::File file = Tests::buildTestFile();
//    QCOMPARE(file.duration().toTimecode().c_str(),"00:00:01");
//    QCOMPARE(file.tracks(),static_cast<size_t>(3));
//}



TEST(Other, NoexceptMoveCtors) {
    // for the best std::vector performance we expect following classes to be noexcept move constructible
    EXPECT_TRUE(std::is_nothrow_move_constructible<CxxMidi::Message>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<CxxMidi::Message>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<CxxMidi::Event>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<CxxMidi::Event>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<CxxMidi::Track>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<CxxMidi::Track>::value);
}
