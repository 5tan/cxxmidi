```
       @ @@@                                       @@@@@   @@    @@                @@
     @  @@@@  @                                 @@@@@@  @@@@@ @@@@@     @           @@     @
    @  @  @@@@                                 @@   @  @  @@@@@ @@@@@  @@@          @@    @@@
   @  @@   @@                                 @    @  @   @ @@  @ @@    @           @@     @
  @  @@@         @@@    @@@     @@@    @@@        @  @    @     @                   @@
 @@   @@        @ @@@  @@@@ @  @ @@@  @@@@ @     @@ @@    @     @     @@@       @@@ @@   @@@
 @@   @@           @@@ @@@@@      @@@ @@@@@      @@ @@    @     @      @@@     @@@@@@@@@  @@@
 @@   @@            @@@  @@        @@@  @@       @@ @@    @     @       @@    @@   @@@@    @@
 @@   @@             @@@            @@@          @@ @@    @     @       @@    @@    @@     @@
 @@   @@            @ @@@          @ @@@         @@ @@    @     @@      @@    @@    @@     @@
  @@  @@           @   @@@        @   @@@        @  @@    @     @@      @@    @@    @@     @@
   @@ @      @    @     @@@      @     @@@          @     @      @@     @@    @@    @@     @@
    @@@     @    @       @@@ @  @       @@@ @   @@@@      @      @@     @@    @@    @@     @@
     @@@@@@@    @         @@@  @         @@@   @  @@@@@           @@    @@@ @  @@@@@       @@@ @
       @@@                                    @     @@                   @@@    @@@         @@@
                                              @
                                               @@
```

# CxxMidi v0.2 [![Build Status](https://travis-ci.org/5tan/cxxmidi.svg?branch=master)](https://travis-ci.org/5tan/cxxmidi)

C++ MIDI library.

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
* C++11 avail
* C++98 compatible

## Hello World example

``` cpp
#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/synchronous.hpp>

int main(int /*argc*/, char ** /*argv*/)
{
    CxxMidi::Output::Default output;
    for(int i=0; i<output.getPortCount(); i++)
        std::cout << i << ": " << output.getPortName(i) << std::endl;
    output.openPort(0);

    CxxMidi::File file("some_file.mid");

    CxxMidi::Player::Synchronous player(&output);
    player.setFile(&file);

    player.setCallbackHeartbeat([&]()
    {
        std::cout << player.currentTimePos() << std::endl;
    });

    player.play();
}
```

## More examples

* `examples/asynchronous` Demonstrates asynchronous MIDI player.
* `examples/callbacks` Demonstrates use of CxxMidi callbacks.
* `examples/qtmidieditor` Simple Qt GUI MIDI editor.
* `examples/qtmidiplayer` Simple Qt GUI MIDI player.
* `examples/sequencing` Demonstrates how to algorithmically create MIDI files.
* `examples/synchronous` Demonstrates synchronous MIDI player.

## How to build

You don't have to build a library in order to use it. Simply clone the repo and copy `include` directory into your project.

Use of some CxxMidi classes requires linking external libraries.

CxxMidi class | Required external library
------------- | -------------------------
`CxxMidi::Output::Alsa` or `CxxMidi::Output::Default` on Unix | libasound
`CxxMidi::Output::Windows` or `CxxMidi::Output::Default` on Windows | winmm.lib
`CxxMidi::Player::Asynchronous` and if pre C++11 compiler on Unix | lpthread


## License

* CxxMidi: GPLv3
* pstdint: BSD
* RtMidi: https://www.music.mcgill.ca/~gary/rtmidi/index.html#license 
