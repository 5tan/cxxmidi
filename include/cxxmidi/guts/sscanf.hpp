#ifndef CXXMIDI_GUTS_SSCANF_HPP
#define CXXMIDI_GUTS_SSCANF_HPP

#include <cstdio>

#ifndef CXXMIDI_SSCANF
#ifdef _WIN32
#define CXXMIDI_SSCANF(cstr,size,fmt,...) sscanf_s(cstr,fmt,__VA_ARGS__,size)
#endif // _WIN32
#ifdef __unix
#define CXXMIDI_SSCANF(cstr,size,fmt,...) std::sscanf(cstr,fmt,__VA_ARGS__,size)
#endif // __unix
#endif

#endif // CXXMIDI_GUTS_SSCANF_HPP
