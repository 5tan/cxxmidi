#ifndef CXXMIDI_CALLBACK_HPP
#define CXXMIDI_CALLBACK_HPP

namespace CxxMidi {

class Callback
{
public:
    inline Callback(){}

    virtual void operator()()=0;
};

} // namespace CxxMidi

#endif // CXXMIDI_CALLBACK_HPP
