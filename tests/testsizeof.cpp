#include <QtTest/QtTest>

#include <cxxmidi/guts/stdint.hpp>

class TestSizeof: public QObject
{
    Q_OBJECT
private slots:
    void run();
};

void TestSizeof::run()
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

QTEST_MAIN(TestSizeof)
#include "testsizeof.moc"
