#include <QtTest/QtTest>
#include <QDebug>

#include <cxxmidi/guts/stdint.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/note.hpp>

class Tests: public QObject
{
    Q_OBJECT

    static CxxMidi::File buildTestFile();

private slots:
    void stdint();
    void file();
};

CxxMidi::File Tests::buildTestFile()
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


void Tests::stdint()
{
    QCOMPARE(sizeof(int8_t), static_cast<std::size_t>(1));
    QCOMPARE(sizeof(uint8_t), static_cast<std::size_t>(1));
    QCOMPARE(sizeof(int16_t), static_cast<std::size_t>(2));
    QCOMPARE(sizeof(uint16_t), static_cast<std::size_t>(2));
    QCOMPARE(sizeof(int32_t), static_cast<std::size_t>(4));
    QCOMPARE(sizeof(uint32_t), static_cast<std::size_t>(4));
    QCOMPARE(sizeof(int64_t), static_cast<std::size_t>(8));
    QCOMPARE(sizeof(uint64_t), static_cast<std::size_t>(8));
}

void Tests::file()
{
    CxxMidi::File file = Tests::buildTestFile();
    QCOMPARE(QString(file.duration().toTimecode().c_str()),QString("00:00:01"));
    QCOMPARE(file.tracks(),static_cast<size_t>(3));
}

QTEST_MAIN(Tests)
#include "tests.moc"
