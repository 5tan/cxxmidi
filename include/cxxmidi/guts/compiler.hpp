#ifndef CXXMIDI_GUTS_COMPILER_HPP
#define CXXMIDI_GUTS_COMPILER_HPP

#ifndef CXXMIDI_CPP11
#   if __cplusplus > 199711L
#       define CXXMIDI_CPP11
#   endif
#endif

#ifdef CXXMIDI_CPP11
#   define CXXMIDI_CONSTEXPR constexpr
#else
#   define CXXMIDI_CONSTEXPR
#endif

#endif // CXXMIDI_GUTS_COMPILER_COMPILER_HPP

