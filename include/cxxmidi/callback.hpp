#ifndef CXXMIDI_CALLBACK_HPP
#define CXXMIDI_CALLBACK_HPP

namespace CxxMidi {

template<typename T>
class Callback
{
public:
    inline Callback(){}
    virtual void operator()(T*)=0;
};

} // namespace CxxMidi

#endif // CXXMIDI_CALLBACK_HPP
