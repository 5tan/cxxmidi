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

#ifndef INCLUDE_CXXMIDI_INSTRUMENT_HPP_
#define INCLUDE_CXXMIDI_INSTRUMENT_HPP_

#include <string>

namespace cxxmidi {

class Instrument {
 public:
  enum Name {
    kAcousticGrandPiano = 0,
    kBrightAcousticPiano = 1,
    kElectricGrandPiano = 2,
    kHonkytonkPiano = 3,
    kElectricPiano1 = 4,
    kElectricPiano2 = 5,
    kHarpsichord = 6,
    kClavi = 7,

    kCelesta = 8,
    kGlockenspiel = 9,
    kMusicBox = 10,
    kVibraphone = 11,
    kMarimba = 12,
    kXylophone = 13,
    kTubularBells = 14,
    kDulcimer = 15,

    kDrawbarOrgan = 16,
    kPercussiveOrgan = 17,
    kRockOrgan = 18,
    kChurchOrgan = 19,
    kReedOrgan = 20,
    kAccordion = 21,
    kHarmonica = 22,
    kTangoAccordion = 23,

    kAcousticGuitarNylon = 24,
    kAcousticGuitarSteel = 25,
    kElectricGuitarJazz = 26,
    kElectricGuitarClean = 27,
    kElectricGuitarMuted = 28,
    kOverdrivenGuitar = 29,
    kDistortionGuitar = 30,
    kGuitarHarmonics = 31,
    kAcousticBass = 32,
    kElectricBassFinger = 33,
    kElectricBassPick = 34,
    kFretlessBass = 35,
    kSlapBass1 = 36,
    kSlapBass2 = 37,
    kSynthBass1 = 38,
    kSynthBass2 = 39,

    kViolin = 40,
    kViola = 41,
    kCello = 42,
    kContrabass = 43,
    kTremoloStrings = 44,
    kPizzacatoStrings = 45,
    kOrchestralHarp = 46,
    kTimpani = 47,

    kStringEnsemble1 = 48,
    kStringEnsemble2 = 49,
    kSynthstrings1 = 50,
    kSynthstrings2 = 51,
    kChoirAahs = 52,
    kVoiceOohs = 53,
    kSynthVoice = 54,
    kOrchestraHit = 55,

    kTrumpet = 56,
    kTrombone = 57,
    kTuba = 58,
    kMutedTrumped = 59,
    kFrenchHorn = 60,
    kBrassSection = 61,
    kSynthbrass1 = 62,
    kSynthbrass2 = 63,
    kSopranoSax = 64,
    kAltoSax = 65,
    kTenorSax = 66,
    kBaritoneSax = 67,
    kOboe = 68,
    kEnglishHorn = 69,
    kBassoon = 70,
    kClarinet = 71,

    kPiccolo = 72,
    kFlute = 73,
    kRecorder = 74,
    kPanFlute = 75,
    kBlownBottle = 76,
    kShakuhachi = 77,
    kWhistle = 78,
    kOcarina = 79,

    kLeadSquare = 80,
    kLeadSawtooth = 81,
    kLeadCalliope = 82,
    kLeadChiff = 83,
    kLeadCharang = 84,
    kLeadVoice = 85,
    kLeadFifths = 86,
    kLeadBass = 87,

    kPadNewAge = 88,
    kPadWarm = 89,
    kPadPolysynth = 90,
    kPadChoir = 91,
    kPadBowed = 92,
    kPadMetallic = 93,
    kPadHalo = 94,
    kPadSweep = 95,

    kFxTrain = 96,
    kFxSoundtrack = 97,
    kFxCrystal = 98,
    kFxAtmosphere = 99,
    kFxBrightness = 100,
    kFxGoblins = 101,
    kFxEchoes = 102,
    kFxScifi = 103,

    kSitar = 104,
    kBanjo = 105,
    kShamisen = 106,
    kKoto = 107,
    kKalimba = 108,
    kBagpipe = 109,
    kFiddle = 110,
    kShanai = 111,

    kTinkleBell = 112,
    kAgogo = 113,
    kSteelDrums = 114,
    kWoodblocks = 115,
    kTaikoDrum = 116,
    kMelodicDrum = 117,
    kSynthDrum = 118,
    kReverseCymbal = 119,

    kGuitarFretNoise = 120,
    kBreathNoise = 121,
    kSeashore = 122,
    kBirdTweet = 123,
    kTelephoneRing = 124,
    kHelicopter = 125,
    kApplause = 126,
    kGunshot = 127,

    kAcousticBassDrum = 35,  // percussion (notes played on channel 10)
    kBassDrum1 = 36,
    kSideStick = 37,
    kAcousticSnare = 38,
    kHandClap = 39,
    kElectricSnare = 40,
    kLowFloorTom = 41,
    kClosedHiHat = 42,
    kHighFloorTom = 43,
    kPedalHiHat = 44,
    kLowTom = 45,
    kOpenHiHat = 46,
    kLowMidTom = 47,
    kHighMidTom = 48,
    kCrashCymbal1 = 49,
    kHighTom = 50,
    kRideCymbal1 = 51,
    kChineseCymbal = 52,
    kRideBell = 53,
    kTambourine = 54,
    kSplashCymbal = 55,
    kCowbell = 56,
    kCrashCymbal2 = 57,
    kVibraslap = 58,
    kRideCymbal2 = 59,
    kHiBongo = 60,
    kLowBongo = 61,
    kMuteHiConga = 62,
    kOpenHiConga = 63,
    kLowConga = 64,
    kHighTimbale = 65,
    kLowTimbale = 66,
    kHighAgogo = 67,
    kLowAgogo = 68,
    kCabasa = 69,
    kMaracas = 70,
    kShortWhistle = 71,
    kLongWhistle = 72,
    kShortGuiro = 73,
    kLongGuiro = 74,
    kClaves = 75,
    kHiWoodBlock = 76,
    kLowWoodBlock = 77,
    kMuteCuica = 78,
    kOpenCuica = 79,
    kMuteTriangle = 80,
    kOpenTriangle = 81,

    kUndefined = 128
  };

  inline Instrument();
  inline explicit Instrument(int val);

  inline operator int() const;

  inline static std::string GetName(int instrument, int channel = -1);

 protected:
  int val_;  // 0 ... 128
};

}  // namespace cxxmidi

namespace cxxmidi {

Instrument::Instrument() : val_(kUndefined) {}

Instrument::Instrument(int val) : val_(val) {}

Instrument::operator int() const { return val_; }

std::string Instrument::GetName(int instrument, int channel) {
  if (channel == 10) {
    switch (instrument) {
      case kAcousticBassDrum:
        return "Acoustic Bass Drum";
      case kBassDrum1:
        return "Bass Drum 1";
      case kSideStick:
        return "Side Stick";
      case kAcousticSnare:
        return "Acoustic Snare";
      case kHandClap:
        return "Hand Clap";
      case kElectricSnare:
        return "Electric Snare";
      case kLowFloorTom:
        return "Low Floor Tom";
      case kClosedHiHat:
        return "Closed Hi Hat";
      case kHighFloorTom:
        return "High Floor Tom";
      case kPedalHiHat:
        return "Pedal Hi Hat";
      case kLowTom:
        return "Low Tom";
      case kOpenHiHat:
        return "Open Hi Hat";
      case kLowMidTom:
        return "Low Mid Tom";
      case kHighMidTom:
        return "High Mid Tom";
      case kCrashCymbal1:
        return "Crash Cymbal 1";
      case kHighTom:
        return "High Tom";
      case kRideCymbal1:
        return "Ride Cymbal 1";
      case kChineseCymbal:
        return "Chinese Cymbal";
      case kRideBell:
        return "Ride Bell";
      case kTambourine:
        return "Tambourine";
      case kSplashCymbal:
        return "Splash Cymbal";
      case kCowbell:
        return "Cowbell";
      case kCrashCymbal2:
        return "Crash Cymbal 2";
      case kVibraslap:
        return "Vibraslap";
      case kRideCymbal2:
        return "Ride Cymbal 2";
      case kHiBongo:
        return "Hi Bongo";
      case kLowBongo:
        return "Low Bongo";
      case kMuteHiConga:
        return "Mute Hi Conga";
      case kOpenHiConga:
        return "Open Hi Conga";
      case kLowConga:
        return "Low Conga";
      case kHighTimbale:
        return "High Timbale";
      case kLowTimbale:
        return "Low Timbale";
      case kHighAgogo:
        return "High Agogo";
      case kLowAgogo:
        return "Low Agogo";
      case kCabasa:
        return "Cabasa";
      case kMaracas:
        return "Maracas";
      case kShortWhistle:
        return "Short Whistle";
      case kLongWhistle:
        return "Long Whistle";
      case kShortGuiro:
        return "Short Guiro";
      case kLongGuiro:
        return "Long Guiro";
      case kClaves:
        return "Claves";
      case kHiWoodBlock:
        return "Hi Wood Block";
      case kLowWoodBlock:
        return "Low Wood Block";
      case kMuteCuica:
        return "Mute Cuica";
      case kOpenCuica:
        return "Open Cuica";
      case kMuteTriangle:
        return "Mute Triangle";
      case kOpenTriangle:
        return "Open Triangle";
      default:
        break;
    }
  }

  // undefined channel
  if (channel == -1) {
    switch (instrument) {
      case kFretlessBass:
        return "Fretless Bass / Acoustic Bass Drum";
      case kSlapBass1:
        return "Slap Bass 1 / Bass Drum1";
      case kSlapBass2:
        return "Slap Bass 2 / Side Stick";
      case kSynthBass1:
        return "Synth Bass 1 / Acoustic Snare";
      case kSynthBass2:
        return "Synth Bass 2 / Hand Clap";
      case kViolin:
        return "Violin / Electric Snare";
      case kViola:
        return "Viola / Low Floor Tom";
      case kCello:
        return "Cello / Closed Hi Hat";
      case kContrabass:
        return "Contrabass / High Floor Tom";
      case kTremoloStrings:
        return "Tremolo Strings / Pedal HiHat";
      case kPizzacatoStrings:
        return "Pizzacato Strings / Low Tom";
      case kOrchestralHarp:
        return "Orchestral Harp / Open Hi Hat";
      case kTimpani:
        return "Timpani / Low Mid Tom";
      case kStringEnsemble1:
        return "String Ensemble 1 / High Mid Tom";
      case kStringEnsemble2:
        return "String Ensemble 2 / Crash Cymbal1";
      case kSynthstrings1:
        return "Synthstrings 1 / High Tom";
      case kSynthstrings2:
        return "Synthstrings 2 / Ride Cymbal1";
      case kChoirAahs:
        return "Choir Aahs / Chinese Cymbal";
      case kVoiceOohs:
        return "Voice Oohs / Ride Bell";
      case kSynthVoice:
        return "Synth Voice / Tambourine";
      case kOrchestraHit:
        return "Orchestra Hit / Splash Cymbal";
      case kTrumpet:
        return "Trumpet / Cowbell";
      case kTrombone:
        return "Trombone / Crash Cymbal2";
      case kTuba:
        return "Tuba / Vibraslap";
      case kMutedTrumped:
        return "Muted Trumped / Ride Cymbal2";
      case kFrenchHorn:
        return "French Horn / Hi Bongo";
      case kBrassSection:
        return "Brass Section / Low Bongo";
      case kSynthbrass1:
        return "Synthbrass 1 / Mute Hi Conga";
      case kSynthbrass2:
        return "Synthbrass 2 / Open Hi Conga";
      case kSopranoSax:
        return "Soprano Sax / Low Conga";
      case kAltoSax:
        return "Alto Sax / High Timbale";
      case kTenorSax:
        return "Tenor Sax / Low Timbale";
      case kBaritoneSax:
        return "Baritone Sax / High Agogo";
      case kOboe:
        return "Oboe / Low Agogo";
      case kEnglishHorn:
        return "English Horn / Cabasa";
      case kBassoon:
        return "Bassoon / Maracas";
      case kClarinet:
        return "Clarinet / Short Whistle";
      case kPiccolo:
        return "Piccolo / Long Whistle";
      case kFlute:
        return "Flute / Short Guiro";
      case kRecorder:
        return "Recorder / Long Guiro";
      case kPanFlute:
        return "Pan Flute / Claves";
      case kBlownBottle:
        return "Blown Bottle / Hi Wood Block";
      case kShakuhachi:
        return "Shakuhachi / Low Wood Block";
      case kWhistle:
        return "Whistle / Mute Cuica";
      case kOcarina:
        return "Ocarina / Open Cuica";
      case kLeadSquare:
        return "Lead Square / Mute Triangle";
      case kLeadSawtooth:
        return "Lead Sawtooth / Open Triangle";
      default:
        break;
    }
  }

  switch (instrument) {
    case kAcousticGrandPiano:
      return "Acoustic Grand Piano";
    case kBrightAcousticPiano:
      return "Bright Acoustic Piano";
    case kElectricGrandPiano:
      return "Electric Grand Piano";
    case kHonkytonkPiano:
      return "Honkytonk Piano";
    case kElectricPiano1:
      return "Electric Piano 1";
    case kElectricPiano2:
      return "Electric Piano 2";
    case kHarpsichord:
      return "Harpsichord";
    case kClavi:
      return "Clavi";
    case kCelesta:
      return "Celesta";
    case kGlockenspiel:
      return "Glockenspiel";
    case kMusicBox:
      return "Music Box";
    case kVibraphone:
      return "Vibraphone";
    case kMarimba:
      return "Marimba";
    case kXylophone:
      return "Xylophone";
    case kTubularBells:
      return "Tubular Bells";
    case kDulcimer:
      return "Dulcimer";
    case kDrawbarOrgan:
      return "Drawbar Organ";
    case kPercussiveOrgan:
      return "Percussive Organ";
    case kRockOrgan:
      return "Rock Organ";
    case kChurchOrgan:
      return "Church Organ";
    case kReedOrgan:
      return "Reed Organ";
    case kAccordion:
      return "Accordion";
    case kHarmonica:
      return "Harmonica";
    case kTangoAccordion:
      return "Tango Accordion";
    case kAcousticGuitarNylon:
      return "Acoustic Guitar Nylon";
    case kAcousticGuitarSteel:
      return "Acoustic Guitar Steel";
    case kElectricGuitarJazz:
      return "Electric Guitar Jazz";
    case kElectricGuitarClean:
      return "Electric Guitar Clean";
    case kElectricGuitarMuted:
      return "Electric Guitar Muted";
    case kOverdrivenGuitar:
      return "Overdriven Guitar";
    case kDistortionGuitar:
      return "Distortion Guitar";
    case kGuitarHarmonics:
      return "Guitar Harmonics";
    case kAcousticBass:
      return "Acoustic Bass";
    case kElectricBassFinger:
      return "Electric Bass Finger";
    case kElectricBassPick:
      return "Electric Bass Pick";
    case kFretlessBass:
      return "Fretless Bass";
    case kSlapBass1:
      return "Slap Bass 1";
    case kSlapBass2:
      return "Slap Bass 2";
    case kSynthBass1:
      return "Synth Bass 1";
    case kSynthBass2:
      return "Synth Bass 2";
    case kViolin:
      return "Violin";
    case kViola:
      return "Viola";
    case kCello:
      return "Cello";
    case kContrabass:
      return "Contrabass";
    case kTremoloStrings:
      return "Tremolo Strings";
    case kPizzacatoStrings:
      return "Pizzacato Strings";
    case kOrchestralHarp:
      return "Orchestral Harp";
    case kTimpani:
      return "Timpani";
    case kStringEnsemble1:
      return "String Ensemble 1";
    case kStringEnsemble2:
      return "String Ensemble 2";
    case kSynthstrings1:
      return "Synthstrings 1";
    case kSynthstrings2:
      return "Synthstrings 2";
    case kChoirAahs:
      return "Choir Aahs";
    case kVoiceOohs:
      return "Voice Oohs";
    case kSynthVoice:
      return "Synth Voice";
    case kOrchestraHit:
      return "Orchestra Hit";
    case kTrumpet:
      return "Trumpet";
    case kTrombone:
      return "Trombone";
    case kTuba:
      return "Tuba";
    case kMutedTrumped:
      return "Muted Trumped";
    case kFrenchHorn:
      return "French Horn";
    case kBrassSection:
      return "Brass Section";
    case kSynthbrass1:
      return "Synthbrass 1";
    case kSynthbrass2:
      return "Synthbrass 2";
    case kSopranoSax:
      return "Soprano Sax";
    case kAltoSax:
      return "Alto Sax";
    case kTenorSax:
      return "Tenor Sax";
    case kBaritoneSax:
      return "Baritone Sax";
    case kOboe:
      return "Oboe";
    case kEnglishHorn:
      return "English Horn";
    case kBassoon:
      return "Bassoon";
    case kClarinet:
      return "Clarinet";
    case kPiccolo:
      return "Piccolo";
    case kFlute:
      return "Flute";
    case kRecorder:
      return "Recorder";
    case kPanFlute:
      return "Pan Flute";
    case kBlownBottle:
      return "Blown Bottle";
    case kShakuhachi:
      return "Shakuhachi";
    case kWhistle:
      return "Whistle";
    case kOcarina:
      return "Ocarina";
    case kLeadSquare:
      return "Lead Square";
    case kLeadSawtooth:
      return "Lead Sawtooth";
    case kLeadCalliope:
      return "Lead Calliope";
    case kLeadChiff:
      return "Lead Chiff";
    case kLeadCharang:
      return "Lead Charang";
    case kLeadVoice:
      return "Lead Voice";
    case kLeadFifths:
      return "Lead Fifths";
    case kLeadBass:
      return "Lead Bass";
    case kPadNewAge:
      return "Pad New Age";
    case kPadWarm:
      return "Pad Warm";
    case kPadPolysynth:
      return "Pad Polysynth";
    case kPadChoir:
      return "Pad Choir";
    case kPadBowed:
      return "Pad Bowed";
    case kPadMetallic:
      return "Pad Metallic";
    case kPadHalo:
      return "Pad Halo";
    case kPadSweep:
      return "Pad Sweep";
    case kFxTrain:
      return "Fx Train";
    case kFxSoundtrack:
      return "Fx Soundtrack";
    case kFxCrystal:
      return "Fx Crystal";
    case kFxAtmosphere:
      return "Fx Atmosphere";
    case kFxBrightness:
      return "Fx Brightness";
    case kFxGoblins:
      return "Fx Goblins";
    case kFxEchoes:
      return "Fx Echoes";
    case kFxScifi:
      return "Fx Sci fi";
    case kSitar:
      return "Sitar";
    case kBanjo:
      return "Banjo";
    case kShamisen:
      return "Shamisen";
    case kKoto:
      return "Koto";
    case kKalimba:
      return "Kalimba";
    case kBagpipe:
      return "Bagpipe";
    case kFiddle:
      return "Fiddle";
    case kShanai:
      return "Shanai";
    case kTinkleBell:
      return "Tinkle Bell";
    case kAgogo:
      return "Agogo";
    case kSteelDrums:
      return "Steel Drums";
    case kWoodblocks:
      return "Woodblocks";
    case kTaikoDrum:
      return "Taiko Drum";
    case kMelodicDrum:
      return "Melodic Drum";
    case kSynthDrum:
      return "Synth Drum";
    case kReverseCymbal:
      return "Reverse Cymbal";
    case kGuitarFretNoise:
      return "Guitar Fret Noise";
    case kBreathNoise:
      return "Breath Noise";
    case kSeashore:
      return "Seashore";
    case kBirdTweet:
      return "Bird Tweet";
    case kTelephoneRing:
      return "Telephone Ring";
    case kHelicopter:
      return "Helicopter";
    case kApplause:
      return "Applause";
    case kGunshot:
      return "Gunshot";
    case kUndefined:
      return "Undefined";
    default:
      return "";
  }
}

}  // namespace cxxmidi

#endif  // INCLUDE_CXXMIDI_INSTRUMENT_HPP_
