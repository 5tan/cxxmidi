#ifndef CXXMIDI_NOTE_HPP
#define CXXMIDI_NOTE_HPP

#include <string>

namespace CxxMidi {

class Note
{
public:

    enum Name
    {
        Undefined = 128,

        // octave -5
        C00     = 0,
        C00s    = 1,
        D00b    = 1,
        D00     = 2,
        D00s    = 3,
        E00b    = 3,
        E00     = 4,
        F00     = 5,
        F00s    = 6,
        G00b    = 6,
        G00     = 7,
        G00s    = 8,
        A00b    = 8,
        A00     = 9,
        A00s    = 10,
        B00b    = 10,
        B00     = 11,

        // octave -4
        C0      = 12,
        C0s     = 13,
        D0b     = 13,
        D0      = 14,
        D0s     = 15,
        E0b     = 15,
        E0      = 16,
        F0      = 17,
        F0s     = 18,
        G0b     = 18,
        G0      = 19,
        G0s     = 20,
        A0b     = 20,
        A0      = 21,
        A0s     = 22,
        B0b     = 22,
        B0      = 23,

        // octave -3
        C1      = 24,
        C1s     = 25,
        D1b     = 25,
        D1      = 26,
        D1s     = 27,
        E1b     = 27,
        E1      = 28,
        F1      = 29,
        F1s     = 30,
        G1b     = 30,
        G1      = 31,
        G1s     = 32,
        A1b     = 32,
        A1      = 33,
        A1s     = 34,
        B1b     = 34,
        B1      = 35,

        // octave -2
        C2      = 36,
        C2s     = 37,
        D2b     = 37,
        D2      = 38,
        D2s     = 39,
        E2b     = 39,
        E2      = 40,
        F2      = 41,
        F2s     = 42,
        G2b     = 42,
        G2      = 43,
        G2s     = 44,
        A2b     = 44,
        A2      = 45,
        A2s     = 46,
        B2b     = 46,
        B2      = 47,

        // octave -1
        C3      = 48,
        C3s     = 49,
        D3b     = 49,
        D3      = 50,
        D3s     = 51,
        E3b     = 51,
        E3      = 52,
        F3      = 53,
        F3s     = 54,
        G3b     = 54,
        G3      = 55,
        G3s     = 56,
        A3b     = 56,
        A3      = 57,
        A3s     = 58,
        B3b     = 58,
        B3      = 59,

        // octave 0
        C4      = 60, // middle C
        C4s     = 61,
        D4b     = 61,
        D4      = 62,
        D4s     = 63,
        E4b     = 63,
        E4      = 64,
        F4      = 65,
        F4s     = 66,
        G4b     = 66,
        G4      = 67,
        G4s     = 68,
        A4b     = 68,
        A4      = 69, // 440Hz
        A4s     = 70,
        B4b     = 70,
        B4      = 71,

        // octave 1
        C5      = 72,
        C5s     = 73,
        D5b     = 73,
        D5      = 74,
        D5s     = 75,
        E5b     = 75,
        E5      = 76,
        F5      = 77,
        F5s     = 78,
        G5b     = 78,
        G5      = 79,
        G5s     = 80,
        A5b     = 80,
        A5      = 81,
        A5s     = 82,
        B5b     = 82,
        B5      = 83,

        // octave 2
        C6      = 84,
        C6s     = 85,
        D6b     = 85,
        D6      = 86,
        D6s     = 87,
        E6b     = 87,
        E6      = 88,
        F6      = 89,
        F6s     = 90,
        G6b     = 90,
        G6      = 91,
        G6s     = 92,
        A6b     = 92,
        A6      = 93,
        A6s     = 94,
        B6b     = 94,
        B6      = 95,

        // octave 3
        C7      = 96,
        C7s     = 97,
        D7b     = 97,
        D7      = 98,
        D7s     = 99,
        E7b     = 99,
        E7      = 100,
        F7      = 101,
        F7s     = 102,
        G7b     = 102,
        G7      = 103,
        G7s     = 104,
        A7b     = 104,
        A7      = 105,
        A7s     = 106,
        B7b     = 106,
        B7      = 107,

        // octave 4
        C8      = 108,
        C8s     = 109,
        D8b     = 109,
        D8      = 110,
        D8s     = 111,
        E8b     = 111,
        E8      = 112,
        F8      = 113,
        F8s     = 114,
        G8b     = 114,
        G8      = 115,
        G8s     = 116,
        A8b     = 116,
        A8      = 117,
        A8s     = 118,
        B8b     = 118,
        B8      = 119,

        // octave 5
        C9      = 120,
        C9s     = 121,
        D9b     = 121,
        D9      = 122,
        D9s     = 123,
        E9b     = 123,
        E9      = 124,
        F9      = 125,
        F9s     = 126,
        G9b     = 126,
        G9      = 127
    };

    inline Note();
    inline Note(int val_);

    inline static Note MiddleC();
    inline static Note A440Hz();

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

Note Note::MiddleC()
{
    return Note(C4);
}

Note Note::A440Hz()
{
    return Note(A4);
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
    case C00s : return "C00s/D00b";
    case D00  : return "D00";
    case D00s : return "D00s/E00b";
    case E00  : return "E00";
    case F00  : return "F00";
    case F00s : return "F00s/G00b";
    case G00  : return "G00";
    case G00s : return "G00s/A00b";
    case A00  : return "A00";
    case A00s : return "A00s/B00b";
    case B00  : return "B00";

    case C0   : return "C0";
    case C0s  : return "C0s/D0b";
    case D0   : return "D0";
    case D0s  : return "D0s/E0b";
    case E0   : return "E0";
    case F0   : return "F0";
    case F0s  : return "F0s/G0b";
    case G0   : return "G0";
    case G0s  : return "G0s/A0b";
    case A0   : return "A0";
    case A0s  : return "A0s/B0b";

    case B0   : return "B0";
    case C1   : return "C1";
    case C1s  : return "C1s/D1b";
    case D1   : return "D1";
    case D1s  : return "D1s/E1b";
    case E1   : return "E1";
    case F1   : return "F1";
    case F1s  : return "F1s/G1b";
    case G1   : return "G1";
    case G1s  : return "G1s/A1b";
    case A1   : return "A1";
    case A1s  : return "A1s/B1b";
    case B1   : return "B1";

    case C2   : return "C2";
    case C2s  : return "C2s/D2b";
    case D2   : return "D2";
    case D2s  : return "D2s/E2b";
    case E2   : return "E2";
    case F2   : return "F2";
    case F2s  : return "F2s/G2b";
    case G2   : return "G2";
    case G2s  : return "G2s/A2b";
    case A2   : return "A2";
    case A2s  : return "A2s/B2b";
    case B2   : return "B2";

    case C3   : return "C3";
    case C3s  : return "C3s/D3b";
    case D3   : return "D3";
    case D3s  : return "D3s/E3b";
    case E3   : return "E3";
    case F3   : return "F3";
    case F3s  : return "F3s/G3b";;
    case G3   : return "G3";
    case G3s  : return "G3s/A3b";
    case A3   : return "A3";
    case A3s  : return "A3s/B3b";
    case B3   : return "B3";

    case C4   : return "C4";
    case C4s  : return "C4s/D4b";
    case D4   : return "D4";
    case D4s  : return "D4s/E4b";
    case E4   : return "E4";
    case F4   : return "F4";
    case F4s  : return "F4s/G4b";
    case G4   : return "G4";
    case G4s  : return "G4s/A4b";
    case A4   : return "A4";
    case A4s  : return "A4s/B4b";
    case B4   : return "B4";

    case C5   : return "C5";
    case C5s  : return "C5s/D5b";
    case D5   : return "D5";
    case D5s  : return "D5s/E5b";
    case E5   : return "E5";
    case F5   : return "F5";
    case F5s  : return "F5s/G5b";
    case G5   : return "G5";
    case G5s  : return "G5s/A5b";
    case A5   : return "A5";
    case A5s  : return "A5s/B5b";
    case B5   : return "B5";

    case C6   : return "C6";
    case C6s  : return "C6s/D6b";
    case D6   : return "D6";
    case D6s  : return "D6s/E6b";
    case E6   : return "E6";
    case F6   : return "F6";
    case F6s  : return "F6s/G6b";
    case G6   : return "G6";
    case G6s  : return "G6s/A6b";
    case A6   : return "A6";
    case A6s  : return "A6s/B6b";
    case B6   : return "B6";

    case C7   : return "C7";
    case C7s  : return "C7s/D7b";
    case D7   : return "D7";
    case D7s  : return "D7s/E7b";
    case E7   : return "E7";
    case F7   : return "F7";
    case F7s  : return "F7s/G7b";
    case G7   : return "G7";
    case G7s  : return "G7s/A7b";
    case A7   : return "A7";
    case A7s  : return "A7s/B7b";
    case B7   : return "B7";

    case C8   : return "C8";
    case C8s  : return "C8s/D8b";
    case D8   : return "D8";
    case D8s  : return "D8s/E8b";
    case E8   : return "E8";
    case F8   : return "F8";
    case F8s  : return "F8s/G8b";
    case G8   : return "G8";
    case G8s  : return "G8s/A8b";
    case A8   : return "A8";
    case A8s  : return "A8s/B8b";
    case B8   : return "B8";

    case C9   : return "C9";
    case C9s  : return "C9s/D9b";
    case D9   : return "D9";
    case D9s  : return "D9s/E9b";
    case E9   : return "E8";
    case F9   : return "F9";
    case F9s  : return "F9s/G9b";
    case G9   : return "G9";

    case Undefined : return"Undefined";
    default: return"Error";
    }
}

} // namespace CxxMidi

#endif // CXXMIDI_NOTE_HPP
