#ifndef CXXMIDI_INSTRUMENT_HPP
#define CXXMIDI_INSTRUMENT_HPP

#include <string>

namespace CxxMidi {

class Instrument
{
public:

    enum Name
    {
        AcousticGrandPiano          = 0,
        BrightAcousticPiano         = 1,
        ElectricGrandPiano          = 2,
        HonkytonkPiano              = 3,
        ElectricPiano1              = 4,
        ElectricPiano2              = 5,
        Harpsichord                 = 6,
        Clavi                       = 7,

        Celesta                     = 8,
        Glockenspiel                = 9,
        MusicBox                    = 10,
        Vibraphone                  = 11,
        Marimba                     = 12,
        Xylophone                   = 13,
        TubularBells                = 14,
        Dulcimer                    = 15,

        DrawbarOrgan                = 16,
        PercussiveOrgan             = 17,
        RockOrgan                   = 18,
        ChurchOrgan                 = 19,
        ReedOrgan                   = 20,
        Accordion                   = 21,
        Harmonica                   = 22,
        TangoAccordion              = 23,

        AcousticGuitarNylon         = 24,
        AcousticGuitarSteel         = 25,
        ElectricGuitarJazz          = 26,
        ElectricGuitarClean         = 27,
        ElectricGuitarMuted         = 28,
        OverdrivenGuitar            = 29,
        DistortionGuitar            = 30,
        GuitarHarmonics             = 31,
        AcousticBass                = 32,
        ElectricBassFinger          = 33,
        ElectricBassPick            = 34,
        FretlessBass                = 35,
        SlapBass1                   = 36,
        SlapBass2                   = 37,
        SynthBass1                  = 38,
        SynthBass2                  = 39,

        Violin                      = 40,
        Viola                       = 41,
        Cello                       = 42,
        Contrabass                  = 43,
        TremoloStrings              = 44,
        PizzacatoStrings            = 45,
        OrchestralHarp              = 46,
        Timpani                     = 47,

        StringEnsemble1             = 48,
        StringEnsemble2             = 49,
        Synthstrings1               = 50,
        Synthstrings2               = 51,
        ChoirAahs                   = 52,
        VoiceOohs                   = 53,
        SynthVoice                  = 54,
        OrchestraHit                = 55,

        Trumpet                     = 56,
        Trombone                    = 57,
        Tuba                        = 58,
        MutedTrumped                = 59,
        FrenchHorn                  = 60,
        BrassSection                = 61,
        Synthbrass1                 = 62,
        Synthbrass2                 = 63,
        SopranoSax                  = 64,
        AltoSax                     = 65,
        TenorSax                    = 66,
        BaritoneSax                 = 67,
        Oboe                        = 68,
        EnglishHorn                 = 69,
        Bassoon                     = 70,
        Clarinet                    = 71,

        Piccolo                     = 72,
        Flute                       = 73,
        Recorder                    = 74,
        PanFlute                    = 75,
        BlownBottle                 = 76,
        Shakuhachi                  = 77,
        Whistle                     = 78,
        Ocarina                     = 79,

        LeadSquare                  = 80,
        LeadSawtooth                = 81,
        LeadCalliope                = 82,
        LeadChiff                   = 83,
        LeadCharang                 = 84,
        LeadVoice                   = 85,
        LeadFifths                  = 86,
        LeadBass                    = 87,

        PadNewAge                   = 88,
        PadWarm                     = 89,
        PadPolysynth                = 90,
        PadChoir                    = 91,
        PadBowed                    = 92,
        PadMetallic                 = 93,
        PadHalo                     = 94,
        PadSweep                    = 95,

        FxTrain                     = 96,
        FxSoundtrack                = 97,
        FxCrystal                   = 98,
        FxAtmosphere                = 99,
        FxBrightness                = 100,
        FxGoblins                   = 101,
        FxEchoes                    = 102,
        FxScifi                     = 103,

        Sitar                       = 104,
        Banjo                       = 105,
        Shamisen                    = 106,
        Koto                        = 107,
        Kalimba                     = 108,
        Bagpipe                     = 109,
        Fiddle                      = 110,
        Shanai                      = 111,

        TinkleBell                  = 112,
        Agogo                       = 113,
        SteelDrums                  = 114,
        Woodblocks                  = 115,
        TaikoDrum                   = 116,
        MelodicDrum                 = 117,
        SynthDrum                   = 118,
        ReverseCymbal               = 119,

        GuitarFretNoise             = 120,
        BreathNoise                 = 121,
        Seashore                    = 122,
        BirdTweet                   = 123,
        TelephoneRing               = 124,
        Helicopter                  = 125,
        Applause                    = 126,
        Gunshot                     = 127,

        AcousticBassDrum            = 35, // percussion (notes played on channel 10)
        BassDrum1                   = 36,
        SideStick                   = 37,
        AcousticSnare               = 38,
        HandClap                    = 39,
        ElectricSnare               = 40,
        LowFloorTom                 = 41,
        ClosedHiHat                 = 42,
        HighFloorTom                = 43,
        PedalHiHat                  = 44,
        LowTom                      = 45,
        OpenHiHat                   = 46,
        LowMidTom                   = 47,
        HighMidTom                  = 48,
        CrashCymbal1                = 49,
        HighTom                     = 50,
        RideCymbal1                 = 51,
        ChineseCymbal               = 52,
        RideBell                    = 53,
        Tambourine                  = 54,
        SplashCymbal                = 55,
        Cowbell                     = 56,
        CrashCymbal2                = 57,
        Vibraslap                   = 58,
        RideCymbal2                 = 59,
        HiBongo                     = 60,
        LowBongo                    = 61,
        MuteHiConga                 = 62,
        OpenHiConga                 = 63,
        LowConga                    = 64,
        HighTimbale                 = 65,
        LowTimbale                  = 66,
        HighAgogo                   = 67,
        LowAgogo                    = 68,
        Cabasa                      = 69,
        Maracas                     = 70,
        ShortWhistle                = 71,
        LongWhistle                 = 72,
        ShortGuiro                  = 73,
        LongGuiro                   = 74,
        Claves                      = 75,
        HiWoodBlock                 = 76,
        LowWoodBlock                = 77,
        MuteCuica                   = 78,
        OpenCuica                   = 79,
        MuteTriangle                = 80,
        OpenTriangle                = 81,

        Undefined                   = 128
    };

    inline Instrument();
    inline Instrument(int val_);

    inline operator int() const;

    inline static std::string name(int instrument_, int channel_=-1);

protected:
    int _val; // 0 ... 128

};

} // namespace CxxMidi

namespace  CxxMidi {

Instrument::Instrument()
    : _val(Undefined)
{

}

Instrument::Instrument(int val_)
    : _val(val_)
{

}

Instrument::operator int() const
{
    return _val;
}

std::string Instrument::name(int instrument_, int channel_)
{
    if(channel_ == 10)
        switch(instrument_)
        {
        case AcousticBassDrum    : return "Acoustic Bass Drum";
        case BassDrum1           : return "Bass Drum 1";
        case SideStick           : return "Side Stick";
        case AcousticSnare       : return "Acoustic Snare";
        case HandClap            : return "Hand Clap";
        case ElectricSnare       : return "Electric Snare";
        case LowFloorTom         : return "Low Floor Tom";
        case ClosedHiHat         : return "Closed Hi Hat";
        case HighFloorTom        : return "High Floor Tom";
        case PedalHiHat          : return "Pedal Hi Hat";
        case LowTom              : return "Low Tom";
        case OpenHiHat           : return "Open Hi Hat";
        case LowMidTom           : return "Low Mid Tom";
        case HighMidTom          : return "High Mid Tom";
        case CrashCymbal1        : return "Crash Cymbal 1";
        case HighTom             : return "High Tom";
        case RideCymbal1         : return "Ride Cymbal 1";
        case ChineseCymbal       : return "Chinese Cymbal";
        case RideBell            : return "Ride Bell";
        case Tambourine          : return "Tambourine";
        case SplashCymbal        : return "Splash Cymbal";
        case Cowbell             : return "Cowbell";
        case CrashCymbal2        : return "Crash Cymbal 2";
        case Vibraslap           : return "Vibraslap";
        case RideCymbal2         : return "Ride Cymbal 2";
        case HiBongo             : return "Hi Bongo";
        case LowBongo            : return "Low Bongo";
        case MuteHiConga         : return "Mute Hi Conga";
        case OpenHiConga         : return "Open Hi Conga";
        case LowConga            : return "Low Conga";
        case HighTimbale         : return "High Timbale";
        case LowTimbale          : return "Low Timbale";
        case HighAgogo           : return "High Agogo";
        case LowAgogo            : return "Low Agogo";
        case Cabasa              : return "Cabasa";
        case Maracas             : return "Maracas";
        case ShortWhistle        : return "Short Whistle";
        case LongWhistle         : return "Long Whistle";
        case ShortGuiro          : return "Short Guiro";
        case LongGuiro           : return "Long Guiro";
        case Claves              : return "Claves";
        case HiWoodBlock         : return "Hi Wood Block";
        case LowWoodBlock        : return "Low Wood Block";
        case MuteCuica           : return "Mute Cuica";
        case OpenCuica           : return "Open Cuica";
        case MuteTriangle        : return "Mute Triangle";
        case OpenTriangle        : return "Open Triangle";
        default: break;
        }

    if(channel_==-1) // undefined channel
        switch(instrument_)
        {
        case FretlessBass        : return "Fretless Bass / Acoustic Bass Drum";
        case SlapBass1           : return "Slap Bass 1 / Bass Drum1";
        case SlapBass2           : return "Slap Bass 2 / Side Stick";
        case SynthBass1          : return "Synth Bass 1 / Acoustic Snare";
        case SynthBass2          : return "Synth Bass 2 / Hand Clap";
        case Violin              : return "Violin / Electric Snare";
        case Viola               : return "Viola / Low Floor Tom";
        case Cello               : return "Cello / Closed Hi Hat";
        case Contrabass          : return "Contrabass / High Floor Tom";
        case TremoloStrings      : return "Tremolo Strings / Pedal HiHat";
        case PizzacatoStrings    : return "Pizzacato Strings / Low Tom";
        case OrchestralHarp      : return "Orchestral Harp / Open Hi Hat";
        case Timpani             : return "Timpani / Low Mid Tom";
        case StringEnsemble1     : return "String Ensemble 1 / High Mid Tom";
        case StringEnsemble2     : return "String Ensemble 2 / Crash Cymbal1";
        case Synthstrings1       : return "Synthstrings 1 / High Tom";
        case Synthstrings2       : return "Synthstrings 2 / Ride Cymbal1";
        case ChoirAahs           : return "Choir Aahs / Chinese Cymbal";
        case VoiceOohs           : return "Voice Oohs / Ride Bell";
        case SynthVoice          : return "Synth Voice / Tambourine";
        case OrchestraHit        : return "Orchestra Hit / Splash Cymbal";
        case Trumpet             : return "Trumpet / Cowbell";
        case Trombone            : return "Trombone / Crash Cymbal2";
        case Tuba                : return "Tuba / Vibraslap";
        case MutedTrumped        : return "Muted Trumped / Ride Cymbal2";
        case FrenchHorn          : return "French Horn / Hi Bongo";
        case BrassSection        : return "Brass Section / Low Bongo";
        case Synthbrass1         : return "Synthbrass 1 / Mute Hi Conga";
        case Synthbrass2         : return "Synthbrass 2 / Open Hi Conga";
        case SopranoSax          : return "Soprano Sax / Low Conga";
        case AltoSax             : return "Alto Sax / High Timbale";
        case TenorSax            : return "Tenor Sax / Low Timbale";
        case BaritoneSax         : return "Baritone Sax / High Agogo";
        case Oboe                : return "Oboe / Low Agogo";
        case EnglishHorn         : return "English Horn / Cabasa";
        case Bassoon             : return "Bassoon / Maracas";
        case Clarinet            : return "Clarinet / Short Whistle";
        case Piccolo             : return "Piccolo / Long Whistle";
        case Flute               : return "Flute / Short Guiro";
        case Recorder            : return "Recorder / Long Guiro";
        case PanFlute            : return "Pan Flute / Claves";
        case BlownBottle         : return "Blown Bottle / Hi Wood Block";
        case Shakuhachi          : return "Shakuhachi / Low Wood Block";
        case Whistle             : return "Whistle / Mute Cuica";
        case Ocarina             : return "Ocarina / Open Cuica";
        case LeadSquare          : return "Lead Square / Mute Triangle";
        case LeadSawtooth        : return "Lead Sawtooth / Open Triangle";
        default:break;
        }

    switch(instrument_)
    {
    case AcousticGrandPiano  : return "Acoustic Grand Piano";
    case BrightAcousticPiano : return "Bright Acoustic Piano";
    case ElectricGrandPiano  : return "Electric Grand Piano";
    case HonkytonkPiano      : return "Honkytonk Piano";
    case ElectricPiano1      : return "Electric Piano 1";
    case ElectricPiano2      : return "Electric Piano 2";
    case Harpsichord         : return "Harpsichord";
    case Clavi               : return "Clavi";
    case Celesta             : return "Celesta";
    case Glockenspiel        : return "Glockenspiel";
    case MusicBox            : return "Music Box";
    case Vibraphone          : return "Vibraphone";
    case Marimba             : return "Marimba";
    case Xylophone           : return "Xylophone";
    case TubularBells        : return "Tubular Bells";
    case Dulcimer            : return "Dulcimer";
    case DrawbarOrgan        : return "Drawbar Organ";
    case PercussiveOrgan     : return "Percussive Organ";
    case RockOrgan           : return "Rock Organ";
    case ChurchOrgan         : return "Church Organ";
    case ReedOrgan           : return "Reed Organ";
    case Accordion           : return "Accordion";
    case Harmonica           : return "Harmonica";
    case TangoAccordion      : return "Tango Accordion";
    case AcousticGuitarNylon : return "Acoustic Guitar Nylon";
    case AcousticGuitarSteel : return "Acoustic Guitar Steel";
    case ElectricGuitarJazz  : return "Electric Guitar Jazz";
    case ElectricGuitarClean : return "Electric Guitar Clean";
    case ElectricGuitarMuted : return "Electric Guitar Muted";
    case OverdrivenGuitar    : return "Overdriven Guitar";
    case DistortionGuitar    : return "Distortion Guitar";
    case GuitarHarmonics     : return "Guitar Harmonics";
    case AcousticBass        : return "Acoustic Bass";
    case ElectricBassFinger  : return "Electric Bass Finger";
    case ElectricBassPick    : return "Electric Bass Pick";
    case FretlessBass        : return "Fretless Bass";
    case SlapBass1           : return "Slap Bass 1";
    case SlapBass2           : return "Slap Bass 2";
    case SynthBass1          : return "Synth Bass 1";
    case SynthBass2          : return "Synth Bass 2";
    case Violin              : return "Violin";
    case Viola               : return "Viola";
    case Cello               : return "Cello";
    case Contrabass          : return "Contrabass";
    case TremoloStrings      : return "Tremolo Strings";
    case PizzacatoStrings    : return "Pizzacato Strings";
    case OrchestralHarp      : return "Orchestral Harp";
    case Timpani             : return "Timpani";
    case StringEnsemble1     : return "String Ensemble 1";
    case StringEnsemble2     : return "String Ensemble 2";
    case Synthstrings1       : return "Synthstrings 1";
    case Synthstrings2       : return "Synthstrings 2";
    case ChoirAahs           : return "Choir Aahs";
    case VoiceOohs           : return "Voice Oohs";
    case SynthVoice          : return "Synth Voice";
    case OrchestraHit        : return "Orchestra Hit";
    case Trumpet             : return "Trumpet";
    case Trombone            : return "Trombone";
    case Tuba                : return "Tuba";
    case MutedTrumped        : return "Muted Trumped";
    case FrenchHorn          : return "French Horn";
    case BrassSection        : return "Brass Section";
    case Synthbrass1         : return "Synthbrass 1";
    case Synthbrass2         : return "Synthbrass 2";
    case SopranoSax          : return "Soprano Sax";
    case AltoSax             : return "Alto Sax";
    case TenorSax            : return "Tenor Sax";
    case BaritoneSax         : return "Baritone Sax";
    case Oboe                : return "Oboe";
    case EnglishHorn         : return "English Horn";
    case Bassoon             : return "Bassoon";
    case Clarinet            : return "Clarinet";
    case Piccolo             : return "Piccolo";
    case Flute               : return "Flute";
    case Recorder            : return "Recorder";
    case PanFlute            : return "Pan Flute";
    case BlownBottle         : return "Blown Bottle";
    case Shakuhachi          : return "Shakuhachi";
    case Whistle             : return "Whistle";
    case Ocarina             : return "Ocarina";
    case LeadSquare          : return "Lead Square";
    case LeadSawtooth        : return "Lead Sawtooth";
    case LeadCalliope        : return "Lead Calliope";
    case LeadChiff           : return "Lead Chiff";
    case LeadCharang         : return "Lead Charang";
    case LeadVoice           : return "Lead Voice";
    case LeadFifths          : return "Lead Fifths";
    case LeadBass            : return "Lead Bass";
    case PadNewAge           : return "Pad New Age";
    case PadWarm             : return "Pad Warm";
    case PadPolysynth        : return "Pad Polysynth";
    case PadChoir            : return "Pad Choir";
    case PadBowed            : return "Pad Bowed";
    case PadMetallic         : return "Pad Metallic";
    case PadHalo             : return "Pad Halo";
    case PadSweep            : return "Pad Sweep";
    case FxTrain             : return "Fx Train";
    case FxSoundtrack        : return "Fx Soundtrack";
    case FxCrystal           : return "Fx Crystal";
    case FxAtmosphere        : return "Fx Atmosphere";
    case FxBrightness        : return "Fx Brightness";
    case FxGoblins           : return "Fx Goblins";
    case FxEchoes            : return "Fx Echoes";
    case FxScifi             : return "Fx Sci fi";
    case Sitar               : return "Sitar";
    case Banjo               : return "Banjo";
    case Shamisen            : return "Shamisen";
    case Koto                : return "Koto";
    case Kalimba             : return "Kalimba";
    case Bagpipe             : return "Bagpipe";
    case Fiddle              : return "Fiddle";
    case Shanai              : return "Shanai";
    case TinkleBell          : return "Tinkle Bell";
    case Agogo               : return "Agogo";
    case SteelDrums          : return "Steel Drums";
    case Woodblocks          : return "Woodblocks";
    case TaikoDrum           : return "Taiko Drum";
    case MelodicDrum         : return "Melodic Drum";
    case SynthDrum           : return "Synth Drum";
    case ReverseCymbal       : return "Reverse Cymbal";
    case GuitarFretNoise     : return "Guitar Fret Noise";
    case BreathNoise         : return "Breath Noise";
    case Seashore            : return "Seashore";
    case BirdTweet           : return "Bird Tweet";
    case TelephoneRing       : return "Telephone Ring";
    case Helicopter          : return "Helicopter";
    case Applause            : return "Applause";
    case Gunshot             : return "Gunshot";
    case Undefined           : return "Undefined";
    default: return "";
    }
}

} // CxxMidi

#endif // CXXMIDI_INSTRUMENT_HPP
