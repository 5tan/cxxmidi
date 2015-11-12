#ifndef CXXMIDI_GUTS_HPP
#define CXXMIDI_GUTS_HPP

#include <cxxmidi/guts/3rdparty/pstdint.h>
#include <cxxmidi/guts/endianness.hpp>

namespace CxxMidi {
namespace Utils {

inline uint32_t extractTempo(uint8_t v0_, uint8_t v1_, uint8_t v2_)
{
    union
    {
        uint32_t oneTempo;
        uint8_t tabTempo[3];
    };

    oneTempo = 0;

#ifdef CXXMIDI_BIG_ENDIAN
    tabTempo[0] = v0_;
    tabTempo[1] = v1_;
    tabTempo[2] = v2_;
#else
#ifdef CXXMIDI_LIL_ENDIAN
    tabTempo[0] = v2_;
    tabTempo[1] = v1_;
    tabTempo[2] = v0_;
#else
#warning "Undefined endianness will be tested in runtime. Please define CXXMIDI_LIL_ENDIAN or CXXMIDI_BIG_ENDIAN."

    static uint32_t num = 1;
    static bool littleEndian = *(uint8_t *)&num == 1;

    if(littleEndian)
    {
        tabTempo[0] = v2_;
        tabTempo[1] = v1_;
        tabTempo[2] = v0_;
    }
    else
    {
        tabTempo[0] = v0_;
        tabTempo[1] = v1_;
        tabTempo[2] = v2_;
    }
#endif
#endif

    return oneTempo;
}

inline unsigned int usPerTick(unsigned int tempo_uspq_, uint16_t timeDiv_)
{
    return tempo_uspq_/timeDiv_;
}

inline uint32_t getVlq(std::fstream &file_)
{
    uint32_t r=0;
    uint8_t c;

    do
    {
        file_.read(reinterpret_cast<char*>(&c), 1);
        r = (r<<7) + (c & 0x7f);
    } while(c & 0x80);

    return r;
}

inline size_t saveVlq(std::ofstream & outputFile_, unsigned int _val)
{
    size_t r =0;
    uint32_t vlq = _val & 0x7f;

    // prepare variable-length quantity
    while((_val >>= 7) > 0)
    {
        vlq <<= 8;
        vlq |= 0x80;
        vlq += (_val & 0x7f);
    }

    // save variable-length quantity
    while(true)
    {
        r++;
        outputFile_.write(reinterpret_cast<char*>(&vlq),1);
        if(vlq & 0x80)
            vlq >>= 8;
        else
            break;
    }

    return r; // return size of vlq
}

} // namespace Utils
} // namespace CxxMidi

#endif // CXXMIDI_GUTS_HPP
