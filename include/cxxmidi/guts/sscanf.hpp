#ifndef CXXMIDI_GUTS_SSCANF_HPP
#define CXXMIDI_GUTS_SSCANF_HPP

#include <cstdio>
#include <cstdarg>
#include <cxxmidi/guts/unused.hpp>

namespace CxxMidi {
namespace Guts {

inline void sscanfWrapper(const char * str_,
                          size_t size_,
                          const char* fmt_, ...)
{
    va_list args;
    va_start(args,fmt_);

#ifdef _WIN32
    sscanf_s(str_,fmt_,args,size_);
#endif // _WIN32
#ifdef __unix
    CXXMIDI_UNUSED(size_);
    std::sscanf(str_,fmt_,args);
#endif // __unix
    va_end(args);
}

} // namespace Guts
} // namespace CxxMidi

#endif // CXXMIDI_GUTS_SSCANF_HPP
