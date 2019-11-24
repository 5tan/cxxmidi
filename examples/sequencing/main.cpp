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

#include <cxxmidi/converters.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/note.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/player_sync.hpp>

int main(int, char **) {
  uint32_t dt;  // quartenote deltatime [ticks]
  // What value should dt be, if we want quarter notes to last 0.5s?

  // Default MIDI time division is 500ticks/quarternote.
  // Default MIDI tempo is 500000us per quarternote
  dt = cxxmidi::converters::Us2dt(500000,  // 0.5s
                                  500000,  // tempo [us/quarternote]
                                  500);    // time division [us/quarternote]

  cxxmidi::File my_file;
  cxxmidi::Track &track = my_file.AddTrack();

  // Chromatic scale starting on C4
  for (int i = 0; i < 13; i++) {
    track.push_back(cxxmidi::Event(0,                          // deltatime
                                   cxxmidi::Message::kNoteOn,  // message type
                                   cxxmidi::Note::MiddleC() + i,  // note
                                   100));  // velocity [0...127]
    track.push_back(cxxmidi::Event(dt,     // deltatime
                                   cxxmidi::Message::kNoteOn,  // message type
                                   cxxmidi::Note::MiddleC() + i,  // note
                                   0));  // velocity=0 => note off
  }
  track.push_back(cxxmidi::Event(0,  // deltatime
                                 cxxmidi::Message::kMeta,
                                 cxxmidi::Message::kEndOfTrack));

  // play the file
  cxxmidi::output::Default output(0);
  cxxmidi::player::PlayerSync player(&output);
  player.SetFile(&my_file);
  player.Play();

  // save the file
  my_file.SaveAs("chromatic.mid");
}
