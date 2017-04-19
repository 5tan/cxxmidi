#ifndef CXXMIDI_OUTPUT_DEFAULT_HPP
#define CXXMIDI_OUTPUT_DEFAULT_HPP

#ifdef _WIN32 // Windows 32-bit and 64-bit
#   include <cxxmidi/output/windows.hpp>
namespace CxxMidi {
namespace Output {
typedef Output::Windows Default;
} // namespace Output
} // namespace CxxMidi
#elif __APPLE__ // Mac OS X
#   error "CxxMidi: OSX Midi output not yet implemented"
#elif __unix // Linux, *BSD, Mac OS X
#   include <cxxmidi/output/linux/alsa.hpp>
namespace CxxMidi {
namespace Output {
typedef Output::Linux::Alsa Default;
} // namespace Output
} // namespace CxxMidi
#else
#   error "CxxMidi: couldn't determine the OS"
#endif

#endif // CXXMIDI_OUTPUT_DEFAULT_HPP
