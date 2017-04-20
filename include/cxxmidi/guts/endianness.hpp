#ifndef CXXMIDI_GUTS_ENDIANNESS_HPP
#define CXXMIDI_GUTS_ENDIANNESS_HPP

#include <cxxmidi/guts/stdint.hpp>
#include <fstream>
#include <iostream>

namespace CxxMidi {
namespace Guts {
namespace Endianness {

inline bool machineIsLittleEndian()
{
    static const uint32_t num = 1;
    static const bool r = *(uint8_t *)&num == 1;
    return r;
    // note: we don't want constexpr here. Library should detect endianness
    // in runtime so cross compilation is possible.
}

template <typename T>
T fix(const T & v_)
{
    T r = 0;

    for (size_t i = 0; i<sizeof(v_); i++)
    {
        const T size = static_cast<T>(sizeof(v_));
        const T it = static_cast<T>(i);

        T mask1 = 0xff << 8*i;
        T byte = ( v_ & mask1 ) >> 8*i ;
        T offset = (size-it-1)*8;
        T mask2 = 0xff << offset;
        r |= ((( byte ) << offset) & mask2);
    }

    return r;
}

template <typename T>
T readBE(std::fstream &file_)
{
    T r;
    file_.read(reinterpret_cast<char*>(&r), sizeof(T));

    if(machineIsLittleEndian())
        r = fix<T>(r);

    return r;
}

template <typename T>
size_t writeBE(std::ofstream &file_, T val_)
{
    size_t size = sizeof(val_);

    if(machineIsLittleEndian())
        val_ = fix<T>(val_);

    file_.write(reinterpret_cast<char*>(&val_),size);

#ifndef CXXMIDI_QUIET
    if(file_.bad())
        std::cerr << "CxxMidi: file write error" << std::endl;
#endif

    return size;
}

} // namespace Endianness
} // namespace Guts
} // namespace CxxMidi

#endif // CXXMIDI_GUTS_ENDIANNESS_HPP
