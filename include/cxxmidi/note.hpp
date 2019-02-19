/* *****************************************************************************
Copyright (c) 2018 Stan Chlebicki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
***************************************************************************** */

#ifndef INCLUDE_CXXMIDI_NOTE_HPP_
#define INCLUDE_CXXMIDI_NOTE_HPP_

#include <string>

namespace cxxmidi {

class Note {
 public:
  enum Name {
    kUndefined = 128,

    // octave -5
    kC00 = 0,
    kC00s = 1,
    kD00b = 1,
    kD00 = 2,
    kD00s = 3,
    kE00b = 3,
    kE00 = 4,
    kF00 = 5,
    kF00s = 6,
    kG00b = 6,
    kG00 = 7,
    kG00s = 8,
    kA00b = 8,
    kA00 = 9,
    kA00s = 10,
    kB00b = 10,
    kB00 = 11,

    // octave -4
    kC0 = 12,
    kC0s = 13,
    kD0b = 13,
    kD0 = 14,
    kD0s = 15,
    kE0b = 15,
    kE0 = 16,
    kF0 = 17,
    kF0s = 18,
    kG0b = 18,
    kG0 = 19,
    kG0s = 20,
    kA0b = 20,
    kA0 = 21,
    kA0s = 22,
    kB0b = 22,
    kB0 = 23,

    // octave -3
    kC1 = 24,
    kC1s = 25,
    kD1b = 25,
    kD1 = 26,
    kD1s = 27,
    kE1b = 27,
    kE1 = 28,
    kF1 = 29,
    kF1s = 30,
    kG1b = 30,
    kG1 = 31,
    kG1s = 32,
    kA1b = 32,
    kA1 = 33,
    kA1s = 34,
    kB1b = 34,
    kB1 = 35,

    // octave -2
    kC2 = 36,
    kC2s = 37,
    kD2b = 37,
    kD2 = 38,
    kD2s = 39,
    kE2b = 39,
    kE2 = 40,
    kF2 = 41,
    kF2s = 42,
    kG2b = 42,
    kG2 = 43,
    kG2s = 44,
    kA2b = 44,
    kA2 = 45,
    kA2s = 46,
    kB2b = 46,
    kB2 = 47,

    // octave -1
    kC3 = 48,
    kC3s = 49,
    kD3b = 49,
    kD3 = 50,
    kD3s = 51,
    kE3b = 51,
    kE3 = 52,
    kF3 = 53,
    kF3s = 54,
    kG3b = 54,
    kG3 = 55,
    kG3s = 56,
    kA3b = 56,
    kA3 = 57,
    kA3s = 58,
    kB3b = 58,
    kB3 = 59,

    // octave 0
    kC4 = 60,  // middle C
    kC4s = 61,
    kD4b = 61,
    kD4 = 62,
    kD4s = 63,
    kE4b = 63,
    kE4 = 64,
    kF4 = 65,
    kF4s = 66,
    kG4b = 66,
    kG4 = 67,
    kG4s = 68,
    kA4b = 68,
    kA4 = 69,  // 440Hz
    kA4s = 70,
    kB4b = 70,
    kB4 = 71,

    // octave 1
    kC5 = 72,
    kC5s = 73,
    kD5b = 73,
    kD5 = 74,
    kD5s = 75,
    kE5b = 75,
    kE5 = 76,
    kF5 = 77,
    kF5s = 78,
    kG5b = 78,
    kG5 = 79,
    kG5s = 80,
    kA5b = 80,
    kA5 = 81,
    kA5s = 82,
    kB5b = 82,
    kB5 = 83,

    // octave 2
    kC6 = 84,
    kC6s = 85,
    kD6b = 85,
    kD6 = 86,
    kD6s = 87,
    kE6b = 87,
    kE6 = 88,
    kF6 = 89,
    kF6s = 90,
    kG6b = 90,
    kG6 = 91,
    kG6s = 92,
    kA6b = 92,
    kA6 = 93,
    kA6s = 94,
    kB6b = 94,
    kB6 = 95,

    // octave 3
    kC7 = 96,
    kC7s = 97,
    kD7b = 97,
    kD7 = 98,
    kD7s = 99,
    kE7b = 99,
    kE7 = 100,
    kF7 = 101,
    kF7s = 102,
    kG7b = 102,
    kG7 = 103,
    kG7s = 104,
    kA7b = 104,
    kA7 = 105,
    kA7s = 106,
    kB7b = 106,
    kB7 = 107,

    // octave 4
    kC8 = 108,
    kC8s = 109,
    kD8b = 109,
    kD8 = 110,
    kD8s = 111,
    kE8b = 111,
    kE8 = 112,
    kF8 = 113,
    kF8s = 114,
    kG8b = 114,
    kG8 = 115,
    kG8s = 116,
    kA8b = 116,
    kA8 = 117,
    kA8s = 118,
    kB8b = 118,
    kB8 = 119,

    // octave 5
    kC9 = 120,
    kC9s = 121,
    kD9b = 121,
    kD9 = 122,
    kD9s = 123,
    kE9b = 123,
    kE9 = 124,
    kF9 = 125,
    kF9s = 126,
    kG9b = 126,
    kG9 = 127
  };

  inline Note();
  inline explicit Note(int val);

  inline static Note MiddleC();
  inline static Note A440Hz();

  inline operator int() const;

  inline static std::string GetName(int note);

 protected:
  int val_;  // 0 ... 127
};

}  // namespace cxxmidi

namespace cxxmidi {

Note::Note() : val_(kUndefined) {}

Note::Note(int val) : val_(val) {}

Note Note::MiddleC() { return Note(kC4); }

Note Note::A440Hz() { return Note(kA4); }

Note::operator int() const { return val_; }

std::string Note::GetName(int note) {
  switch (note) {
    case kC00:
      return "C00";
    case kC00s:
      return "C00s/D00b";
    case kD00:
      return "D00";
    case kD00s:
      return "D00s/E00b";
    case kE00:
      return "E00";
    case kF00:
      return "F00";
    case kF00s:
      return "F00s/G00b";
    case kG00:
      return "G00";
    case kG00s:
      return "G00s/A00b";
    case kA00:
      return "A00";
    case kA00s:
      return "A00s/B00b";
    case kB00:
      return "B00";

    case kC0:
      return "C0";
    case kC0s:
      return "C0s/D0b";
    case kD0:
      return "D0";
    case kD0s:
      return "D0s/E0b";
    case kE0:
      return "E0";
    case kF0:
      return "F0";
    case kF0s:
      return "F0s/G0b";
    case kG0:
      return "G0";
    case kG0s:
      return "G0s/A0b";
    case kA0:
      return "A0";
    case kA0s:
      return "A0s/B0b";

    case kB0:
      return "B0";
    case kC1:
      return "C1";
    case kC1s:
      return "C1s/D1b";
    case kD1:
      return "D1";
    case kD1s:
      return "D1s/E1b";
    case kE1:
      return "E1";
    case kF1:
      return "F1";
    case kF1s:
      return "F1s/G1b";
    case kG1:
      return "G1";
    case kG1s:
      return "G1s/A1b";
    case kA1:
      return "A1";
    case kA1s:
      return "A1s/B1b";
    case kB1:
      return "B1";

    case kC2:
      return "C2";
    case kC2s:
      return "C2s/D2b";
    case kD2:
      return "D2";
    case kD2s:
      return "D2s/E2b";
    case kE2:
      return "E2";
    case kF2:
      return "F2";
    case kF2s:
      return "F2s/G2b";
    case kG2:
      return "G2";
    case kG2s:
      return "G2s/A2b";
    case kA2:
      return "A2";
    case kA2s:
      return "A2s/B2b";
    case kB2:
      return "B2";

    case kC3:
      return "C3";
    case kC3s:
      return "C3s/D3b";
    case kD3:
      return "D3";
    case kD3s:
      return "D3s/E3b";
    case kE3:
      return "E3";
    case kF3:
      return "F3";
    case kF3s:
      return "F3s/G3b";
    case kG3:
      return "G3";
    case kG3s:
      return "G3s/A3b";
    case kA3:
      return "A3";
    case kA3s:
      return "A3s/B3b";
    case kB3:
      return "B3";

    case kC4:
      return "C4";
    case kC4s:
      return "C4s/D4b";
    case kD4:
      return "D4";
    case kD4s:
      return "D4s/E4b";
    case kE4:
      return "E4";
    case kF4:
      return "F4";
    case kF4s:
      return "F4s/G4b";
    case kG4:
      return "G4";
    case kG4s:
      return "G4s/A4b";
    case kA4:
      return "A4";
    case kA4s:
      return "A4s/B4b";
    case kB4:
      return "B4";

    case kC5:
      return "C5";
    case kC5s:
      return "C5s/D5b";
    case kD5:
      return "D5";
    case kD5s:
      return "D5s/E5b";
    case kE5:
      return "E5";
    case kF5:
      return "F5";
    case kF5s:
      return "F5s/G5b";
    case kG5:
      return "G5";
    case kG5s:
      return "G5s/A5b";
    case kA5:
      return "A5";
    case kA5s:
      return "A5s/B5b";
    case kB5:
      return "B5";

    case kC6:
      return "C6";
    case kC6s:
      return "C6s/D6b";
    case kD6:
      return "D6";
    case kD6s:
      return "D6s/E6b";
    case kE6:
      return "E6";
    case kF6:
      return "F6";
    case kF6s:
      return "F6s/G6b";
    case kG6:
      return "G6";
    case kG6s:
      return "G6s/A6b";
    case kA6:
      return "A6";
    case kA6s:
      return "A6s/B6b";
    case kB6:
      return "B6";

    case kC7:
      return "C7";
    case kC7s:
      return "C7s/D7b";
    case kD7:
      return "D7";
    case kD7s:
      return "D7s/E7b";
    case kE7:
      return "E7";
    case kF7:
      return "F7";
    case kF7s:
      return "F7s/G7b";
    case kG7:
      return "G7";
    case kG7s:
      return "G7s/A7b";
    case kA7:
      return "A7";
    case kA7s:
      return "A7s/B7b";
    case kB7:
      return "B7";

    case kC8:
      return "C8";
    case kC8s:
      return "C8s/D8b";
    case kD8:
      return "D8";
    case kD8s:
      return "D8s/E8b";
    case kE8:
      return "E8";
    case kF8:
      return "F8";
    case kF8s:
      return "F8s/G8b";
    case kG8:
      return "G8";
    case kG8s:
      return "G8s/A8b";
    case kA8:
      return "A8";
    case kA8s:
      return "A8s/B8b";
    case kB8:
      return "B8";

    case kC9:
      return "C9";
    case kC9s:
      return "C9s/D9b";
    case kD9:
      return "D9";
    case kD9s:
      return "D9s/E9b";
    case kE9:
      return "E8";
    case kF9:
      return "F9";
    case kF9s:
      return "F9s/G9b";
    case kG9:
      return "G9";

    case kUndefined:
      return "Undefined";
    default:
      return "Error";
  }
}

}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_NOTE_HPP_
