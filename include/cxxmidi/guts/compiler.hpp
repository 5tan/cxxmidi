#ifndef CXXMIDI_GUTS_COMPILER_HPP
#define CXXMIDI_GUTS_COMPILER_HPP

#if __cplusplus > 199711L
#   define CXXMIDI_CONSTEXPR constexpr
#   define CXXMIDI_NOEXCEPT noexcept
#else
#   define CXXMIDI_CONSTEXPR
#   define CXXMIDI_NOEXCEPT throws()
#endif // __cplusplus > 199711L

#endif // CXXMIDI_GUTS_COMPILER_COMPILER_HPP

