#include <cxxmidi/converters.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/note.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/synchronous.hpp>

int main(int /*argc*/, char ** /*argv*/) {
  uint32_t dt;  // quartenote deltatime [ticks]
  // What value should dt be, if we want quarter notes to last 0.5s?

  // Default MIDI time division is 500ticks/quarternote.
  // Default MIDI tempo is 500000us per quarternote
  dt = cxxmidi::converters::us2dt(500000,  // 0.5s
                                  500000,  // tempo [us/quarternote]
                                  500);    // time division [us/quarternote]

  cxxmidi::File myFile;
  cxxmidi::Track &track = myFile.addTrack();

  // Chromatic scale starting on C4
  for (int i = 0; i < 13; i++) {
    track.push_back(cxxmidi::Event(0,                         // deltatime
                                   cxxmidi::Message::NoteOn,  // message type
                                   cxxmidi::Note::MiddleC() + i,  // note
                                   100));  // velocity [0...127]
    track.push_back(cxxmidi::Event(dt,     // deltatime
                                   cxxmidi::Message::NoteOn,  // message type
                                   cxxmidi::Note::MiddleC() + i,  // note
                                   0));  // velocity=0 => note off
  }
  track.push_back(cxxmidi::Event(0,  // deltatime
                                 cxxmidi::Message::Meta,
                                 cxxmidi::Message::EndOfTrack));

  // play the file
  cxxmidi::output::Default output(0);
  cxxmidi::player::Synchronous player(&output);
  player.setFile(&myFile);
  player.play();

  // save the file
  myFile.saveAs("chromatic.mid");
}
