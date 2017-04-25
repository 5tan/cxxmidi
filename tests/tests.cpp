#include <QtTest/QtTest>
#include <QDebug>

#include <cxxmidi/guts/stdint.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/note.hpp>
#include <cxxmidi/instrument.hpp>
#include <cxxmidi/version.hpp>

class Tests: public QObject
{
    Q_OBJECT

    static CxxMidi::File buildTestFile();

private slots:
    void initTestCase();
    void stdint();
    void time();
    void file();
    void instrument();
//#if __cplusplus > 199711L
    void noexceptMoveCtors();
//#endif // __cplusplus > 199711L

};

void Tests::initTestCase()
{
    qDebug() << "CxxMidi ver." << CXXMIDI_VERSION;
}

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

void Tests::time()
{
    CxxMidi::Time::Point p1 = CxxMidi::Time::Point::fromTimecode("00:00:00");
    CxxMidi::Time::Point p2 = CxxMidi::Time::Point::fromS(1);
    QCOMPARE((p1+p2).toTimecode().c_str(),"00:00:01");
    QCOMPARE((p1-p2).toTimecode().c_str(),"-00:00:01");
    QCOMPARE((p2*2.).toTimecode().c_str(),"00:00:02");
    QCOMPARE((3.*p2).toTimecode().c_str(),"00:00:03");

}

void Tests::file()
{
    CxxMidi::File file = Tests::buildTestFile();
    QCOMPARE(file.duration().toTimecode().c_str(),"00:00:01");
    QCOMPARE(file.tracks(),static_cast<size_t>(3));
}

void Tests::instrument()
{
    QCOMPARE(CxxMidi::Instrument::name(0).c_str(),
             "Acoustic Grand Piano");
    QCOMPARE(CxxMidi::Instrument::name(35).c_str(),
             "Fretless Bass / Acoustic Bass Drum");
    QCOMPARE(CxxMidi::Instrument::name(35,0).c_str(),
             "Fretless Bass");
    QCOMPARE(CxxMidi::Instrument::name(35,10).c_str(),
             "Acoustic Bass Drum");
    QCOMPARE(CxxMidi::Instrument::name(127).c_str(),
             "Gunshot");
    QCOMPARE((int)CxxMidi::Instrument(CxxMidi::Instrument::Gunshot),
             127);
}

void Tests::noexceptMoveCtors()
{
#if __cplusplus > 199711L
    // we want these classes to be noexcept move constructible
    // for the best std::vector performance
    QVERIFY(std::is_nothrow_move_constructible<CxxMidi::Message>::value);
    QVERIFY(std::is_nothrow_move_assignable<CxxMidi::Message>::value);
    QVERIFY(std::is_nothrow_move_constructible<CxxMidi::Event>::value);
    QVERIFY(std::is_nothrow_move_assignable<CxxMidi::Event>::value);
    QVERIFY(std::is_nothrow_move_constructible<CxxMidi::Track>::value);
    QVERIFY(std::is_nothrow_move_assignable<CxxMidi::Track>::value);
#endif // __cplusplus > 199711L
}


QTEST_MAIN(Tests)
#include "tests.moc"
