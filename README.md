# cxxmidi 

C++ MIDI library.

[![Build Status](https://travis-ci.org/5tan/cxxmidi.svg?branch=master)](https://travis-ci.org/5tan/cxxmidi) [![codecov](https://codecov.io/gh/5tan/cxxmidi/branch/master/graph/badge.svg)](https://codecov.io/gh/5tan/cxxmidi)

## Key features
* MIDI files read/write
* MIDI files parsing, editing, sequencing (composition)
* MIDI outputs
  * ALSA on Linux
  * WinMM on Windows
* Multiplatform MIDI players
  * synchronous
  * asynchronous

## Design
* Multiplatform (Linux, Windows)
* Endian safe
* Boost-like HPP header files only library

## Simple example

``` cpp
#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/synchronous.hpp>

int main(int /*argc*/, char** /*argv*/) {
  cxxmidi::output::Default output;
  for (size_t i = 0; i < output.GetPortCount(); i++)
    std::cout << i << ": " << output.GetPortName(i) << std::endl;
  output.OpenPort(1);

  cxxmidi::File file("music/chopin.mid");

  cxxmidi::player::Synchronous player(&output);
  player.SetFile(&file);

  player.SetCallbackHeartbeat(
      [&]() { std::cout << player.CurrentTimePos().count() << std::endl; });

  player.Play();
}
```

## More examples

* `examples/callbacks` Use of CxxMidi callbacks.
* `examples/qtmidieditor` Simple Qt GUI MIDI editor.
* `examples/qtmidiplayer` Simple Qt GUI MIDI player.
* `examples/player_async` Asynchronous MIDI player.
* `examples/player_sync` Synchronous MIDI player.
* `examples/sequencing` How to algorithmically create MIDI files.

## How to use

To use library clone the repo and copy `include` directory into your project.
Use of some cxxmidi classes requires external libraries linkage.

Class | Required external library
--- | ---
`cxxmidi::output::Alsa` | `libasound`
`cxxmidi::output::Windows` | `winmm.lib`

## How to build tests and examples

```
mkdir build
cd build
cmake ..
make
```

## cpplint suppressions

`cpplint` suppressions in the code are marked:
```
// NOLINT(${checkName}) ${mark}
```

Mark | Related check name | Reason for suppression
--- | --- | ---
`CPP11_INCLUDES` | `build/c++11` | `[build/c++11] [5]` is reported when parsing include directive of unapproved C++11 header file (like `chrono`, `thread` or `mutex`). These files have custom implementations in Chrome, but not in this project.
`INCLUDE_NO_DIR` | `build/include_subdir` |`[build/include_subdir] [4]` is reported if no directory name is present in include directive, but it is common in Qt code to skip it. This exception is allowed only in examples code.
`SIGNAL_SLOT_SPECIFIER` | `[whitespace/indent]` | `[whitespace/indent] [3]` is reported when parsing Qt `signal`/`slot` specifiers. 

## cppcheck suppressions

`cppcheck` suppressions in the code are marked:
```
// cppcheck-suppress ${checkName} ${mark}
```

Mark | Related check name | Reason for suppression
--- | --- | ---
`RAII` | `unreadVariable` | `(style) Variable '...' is assigned a value that is never used.` is reported for RAII objects like mutexes.
`LIB_FUNC` | `unusedFunction` | `(style) The function '...' is never used.` is reported for library functions.

## License

* cxxmidi: MIT
* RtMidi: https://www.music.mcgill.ca/~gary/rtmidi/index.html#license 
