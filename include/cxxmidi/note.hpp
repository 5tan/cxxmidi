#ifndef CXXMIDI_NOTE_HPP
#define CXXMIDI_NOTE_HPP

#include <string>

namespace CxxMidi {

class Note
{
public:

  enum
     {
       Undefined = 128,

       C00    = 0,
       Cs00   = 1,
       Db00   = 1,
       D00    = 2,
       Ds00   = 3,
       Eb00   = 3,
       E00    = 4,
       F00    = 5,
       Fs00   = 6,
       Gb00   = 6,
       G00    = 7,
       Gs00   = 8,
       Ab00   = 8,
       A00    = 9,
       As00   = 10,
       Bb00   = 10,
       B00    = 11,

       C0     = 12,
       Cs0    = 13,
       Db0    = 13,
       D0     = 14,
       Ds0    = 15,
       Eb0    = 15,
       E0     = 16,
       F0     = 17,
       Fs0    = 18,
       Gb0    = 18,
       G0     = 19,
       Gs0    = 20,
       Ab0    = 20,
       A0     = 21,
       As0    = 22,
       Bb0    = 22,
       B0     = 23,

       C1     = 24,
       Cs1    = 25,
       Db1    = 25,
       D1     = 26,
       Ds1    = 27,
       Eb1    = 27,
       E1     = 28,
       F1     = 29,
       Fs1    = 30,
       Gb1    = 30,
       G1     = 31,
       Gs1    = 32,
       Ab1    = 32,
       A1     = 33,
       As1    = 34,
       Bb1    = 34,
       B1     = 35,

       C2     = 36,
       Cs2    = 37,
       Db2    = 37,
       D2     = 38,
       Ds2    = 39,
       Eb2    = 39,
       E2     = 40,
       F2     = 41,
       Fs2    = 42,
       Gb2    = 42,
       G2     = 43,
       Gs2    = 44,
       Ab2    = 44,
       A2     = 45,
       As2    = 46,
       Bb2    = 46,
       B2     = 47,

       C3     = 48,
       Cs3    = 49,
       Db3    = 49,
       D3     = 50,
       Ds3    = 51,
       Eb3    = 51,
       E3     = 52,
       F3     = 53,
       Fs3    = 54,
       Gb3    = 54,
       G3     = 55,
       Gs3    = 56,
       Ab3    = 56,
       A3     = 57,
       As3    = 58,
       Bb3    = 58,
       B3     = 59,

       C4     = 60, // middle C
       Cs4    = 61,
       Db4    = 61,
       D4     = 62,
       Ds4    = 63,
       Eb4    = 63,
       E4     = 64,
       F4     = 65,
       Fs4    = 66,
       Gb4    = 66,
       G4     = 67,
       Gs4    = 68,
       Ab4    = 68,
       A4     = 69, // 440 Hz
       As4    = 70,
       Bb4    = 70,
       B4     = 71,

       C5     = 72,
       Cs5    = 73,
       Db5    = 73,
       D5     = 74,
       Ds5    = 75,
       Eb5    = 75,
       E5     = 76,
       F5     = 77,
       Fs5    = 78,
       Gb5    = 78,
       G5     = 79,
       Gs5    = 80,
       Ab5    = 81,
       A5     = 81,
       As5    = 82,
       Bb5    = 82,
       B5     = 83,

       C6     = 84,
       Cs6    = 85,
       Db6    = 85,
       D6     = 86,
       Ds6    = 87,
       Eb6    = 87,
       E6     = 88,
       F6     = 89,
       Fs6    = 90,
       Gb6    = 90,
       G6     = 91,
       Gs6    = 92,
       Ab6    = 92,
       A6     = 93,
       As6    = 94,
       Bb6    = 94,
       B6     = 95,

       C7     = 96,
       Cs7    = 97,
       Db7    = 97,
       D7     = 98,
       Ds7    = 99,
       Eb7    = 99,
       E7     = 100,
       F7     = 101,
       Fs7    = 102,
       Gb7    = 102,
       G7     = 103,
       Gs7    = 104,
       Ab7    = 104,
       A7     = 105,
       As7    = 106,
       Bb7    = 106,
       B7     = 107,

       C8     = 108,
       Cs8    = 109,
       Db8    = 109,
       D8     = 110,
       Ds8    = 111,
       Eb8    = 111,
       E8     = 112,
       F8     = 113,
       Fs8    = 114,
       Gb8    = 114,
       G8     = 115,
       Gs8    = 116,
       Ab8    = 116,
       A8     = 117,
       As8    = 118,
       Bb8    = 118,
       B8     = 119,

       C9     = 120,
       Cs9    = 121,
       Db9    = 121,
       D9     = 122,
       Ds9    = 123,
       Eb9    = 123,
       E9     = 124,
       F9     = 125,
       Fs9    = 126,
       Gb9    = 126,
       G9     = 127
     };

  inline Note();
  inline Note(int val_);

  inline operator int() const;

  inline static std::string name(int note_);

protected:
  int _val; // 0 ... 127
};

} // namespace CxxMidi

namespace CxxMidi {

Note::Note()
    : _val(Undefined)
{

}

Note::Note(int val_)
    : _val(val_)
{

}

Note::operator int() const
{
    return _val;
}

std::string Note::name(int note_)
{
    switch(note_)
    {
    case C00  : return "C00";
    case Cs00 : return "Cs00";
    //case Db00 : return "Db00";
    case D00  : return "D00";
    case Ds00 : return "Ds00";
    //case Eb00 : return "Eb00";
    case E00  : return "E00";
    case F00  : return "F00";
    case Fs00 : return "Fs00";
    //case Gb00 : return "Gb00";
    case G00  : return "G00";
    case Gs00 : return "Gs00";
    //case Ab00 : return "Ab00";
    case A00  : return "A00";
    case As00 : return "As00";
    //case Bb00 : return "Bb00";
    case B00  : return "B00";
    case C0   : return "C0";
    case Cs0  : return "Cs0";
    //case Db0  : return "Db0";
    case D0   : return "D0";
    case Ds0  : return "Ds0";
    //case Eb0  : return "Eb0";
    case E0   : return "E0";
    case F0   : return "F0";
    case Fs0  : return "Fs0";
    //case Gb0  : return "Gb0";
    case G0   : return "G0";
    case Gs0  : return "Gs0";
    //case Ab0  : return "Ab0";
    case A0   : return "A0";
    case As0  : return "As0";
    //case Bb0  : return "Bb0";
    case B0   : return "B0";
    case C1   : return "C1";
    case Cs1  : return "Cs1";
    //case Db1  : return "Db1";
    case D1   : return "D1";
    case Ds1  : return "Ds1";
    //case Eb1  : return "Eb1";
    case E1   : return "E1";
    case F1   : return "F1";
    case Fs1  : return "Fs1";
    //case Gb1  : return "Gb1";
    case G1   : return "G1";
    case Gs1  : return "Gs1";
    //case Ab1  : return "Ab1";
    case A1   : return "A1";
    case As1  : return "As1";
    //case Bb1  : return "Bb1";
    case B1   : return "B1";
    case C2   : return "C2";
    case Cs2  : return "Cs2";
    //case Db2  : return "Db2";
    case D2   : return "D2";
    case Ds2  : return "Ds2";
    //case Eb2  : return "Eb2";
    case E2   : return "E2";
    case F2   : return "F2";
    case Fs2  : return "Fs2";
    //case Gb2  : return "Gb2";
    case G2   : return "G2";
    case Gs2  : return "Gs2";
    //case Ab2  : return "Ab2";
    case A2   : return "A2";
    case As2  : return "As2";
    //case Bb2  : return "Bb2";
    case B2   : return "B2";
    case C3   : return "C3";
    case Cs3  : return "Cs3";
    //case Db3  : return "Db3";
    case D3   : return "D3";
    case Ds3  : return "Ds3";
    //case Eb3  : return "Eb3";
    case E3   : return "E3";
    case F3   : return "F3";
    case Fs3  : return "Fs3";
    //case Gb3  : return "Gb3";
    case G3   : return "G3";
    case Gs3  : return "Gs3";
    //case Ab3  : return "Ab3";
    case A3   : return "A3";
    case As3  : return "As3";
    //case Bb3  : return "Bb3";
    case B3   : return "B3";
    case C4   : return "C4";
    case Cs4  : return "Cs4";
    //case Db4  : return "Db4";
    case D4   : return "D4";
    case Ds4  : return "Ds4";
    //case Eb4  : return "Eb4";
    case E4   : return "E4";
    case F4   : return "F4";
    case Fs4  : return "Fs4";
    //case Gb4  : return "Gb4";
    case G4   : return "G4";
    case Gs4  : return "Gs4";
    //case Ab4  : return "Ab4";
    case A4   : return "A4";
    case As4  : return "As4";
    //case Bb4  : return "Bb4";
    case B4   : return "B4";
    case C5   : return "C5";
    case Cs5  : return "Cs5";
    //case Db5  : return "Db5";
    case D5   : return "D5";
    case Ds5  : return "Ds5";
    //case Eb5  : return "Eb5";
    case E5   : return "E5";
    case F5   : return "F5";
    case Fs5  : return "Fs5";
    //case Gb5  : return "Gb5";
    case G5   : return "G5";
    case Gs5  : return "Gs5";
    case Ab5  : return "Ab5";
    //case A5   : return "A5";
    case As5  : return "As5";
    //case Bb5  : return "Bb5";
    case B5   : return "B5";
    case C6   : return "C6";
    case Cs6  : return "Cs6";
    //case Db6  : return "Db6";
    case D6   : return "D6";
    case Ds6  : return "Ds6";
    //case Eb6  : return "Eb6";
    case E6   : return "E6";
    case F6   : return "F6";
    case Fs6  : return "Fs6";
    //case Gb6  : return "Gb6";
    case G6   : return "G6";
    case Gs6  : return "Gs6";
    //case Ab6  : return "Ab6";
    case A6   : return "A6";
    case As6  : return "As6";
    //case Bb6  : return "Bb6";
    case B6   : return "B6";
    case C7   : return "C7";
    case Cs7  : return "Cs7";
    //case Db7  : return "Db7";
    case D7   : return "D7";
    case Ds7  : return "Ds7";
    //case Eb7  : return "Eb7";
    case E7   : return "E7";
    case F7   : return "F7";
    case Fs7  : return "Fs7";
    //case Gb7  : return "Gb7";
    case G7   : return "G7";
    case Gs7  : return "Gs7";
    //case Ab7  : return "Ab7";
    case A7   : return "A7";
    case As7  : return "As7";
    //case Bb7  : return "Bb7";
    case B7   : return "B7";
    case C8   : return "C8";
    case Cs8  : return "Cs8";
    //case Db8  : return "Db8";
    case D8   : return "D8";
    case Ds8  : return "Ds8";
    //case Eb8  : return "Eb8";
    case E8   : return "E8";
    case F8   : return "F8";
    case Fs8  : return "Fs8";
    //case Gb8  : return "Gb8";
    case G8   : return "G8";
    case Gs8  : return "Gs8";
    //case Ab8  : return "Ab8";
    case A8   : return "A8";
    case As8  : return "As8";
    //case Bb8  : return "Bb8";
    case B8   : return "B8";
    case C9   : return "C9";
    case Cs9  : return "Cs9";
    //case Db9  : return "Db9";
    case D9   : return "D9";
    case Ds9  : return "Ds9";
    //case Eb9  : return "Eb9";
    case E9   : return "E9";
    case F9   : return "F9";
    case Fs9  : return "Fs9";
    //case Gb9  : return "Gb9";
    case G9   : return "G9";
    case Undefined : return "Undefined";
    default: return "Undefined";
    }
}

} // namespace CxxMidi

#endif // CXXMIDI_NOTE_HPP
