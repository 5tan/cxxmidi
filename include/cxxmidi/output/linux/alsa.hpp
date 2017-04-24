// RtMidi: realtime MIDI i/o C++ classes
// Copyright (c) 2003-2011 Gary P. Scavone
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation files
// (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// Any person wishing to distribute modifications to the Software is
// requested to send the modifications to the original developer so that
// they can be incorporated into the canonical version.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef CXXMIDI_OUTPUT_LINUX_ALSA_HPP
#define CXXMIDI_OUTPUT_LINUX_ALSA_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <pthread.h>
#include <sys/time.h>
#include <alsa/asoundlib.h>

#include <cxxmidi/guts/stdint.hpp>
#include <cxxmidi/output/abstract.hpp>

namespace CxxMidi {
namespace Output {
namespace Linux {

class Alsa : public Output::Abstract
{
    struct AlsaMidiData {
        snd_seq_t *seq;
        int vport;
        snd_seq_port_subscribe_t *subscription;
        snd_midi_event_t *coder;
        unsigned int bufferSize;
        unsigned char *buffer;
        pthread_t thread;
        // unsigned long long lastTime;
        unsigned long lastTime;
        int queue_id; // an input queue is needed to get timestamped events
    };

public:
    inline Alsa();
    inline Alsa(unsigned int initialPort_);
    inline virtual ~Alsa();

    Alsa(const Alsa&); // non-copyable
    Alsa &operator=(const Alsa &); // non-copyable (assignment)
#if __cplusplus > 199711L
    Alsa(Alsa&&) = default;
    Alsa& operator=(Alsa&&) = default;
#endif // __cplusplus > 199711L

    inline virtual void openPort( unsigned int portNumber_ = 0);
    inline virtual void closePort();
    inline virtual void openVirtualPort(const std::string& portName_ = std::string( "RtMidi Output"));
    inline virtual size_t getPortCount();
    inline virtual std::string getPortName(unsigned int portNumber_ = 0 );
    inline virtual void sendMessage(const std::vector<uint8_t> *msg_ );

protected:

    inline virtual void initialize();

private:
    inline static size_t portInfo( snd_seq_t *seq, snd_seq_port_info_t *pinfo, unsigned int type, int portNumber );
    AlsaMidiData * _apiData;
};

} // namespace Linux
} // namespace Output
} // namespace CxxMidi

namespace CxxMidi {
namespace Output {
namespace Linux {

Alsa::Alsa()
    : _apiData(0)
{
    this->initialize();
}

Alsa::Alsa(unsigned int initialPort_)
    : _apiData(0)
{
    this->initialize();
    this->openPort(initialPort_);
}

Alsa::~Alsa()
{
    // Close a connection if it exists.
    closePort();

    // Cleanup.
    if ( _apiData->vport >= 0 ) snd_seq_delete_port( _apiData->seq, _apiData->vport );
    if ( _apiData->coder ) snd_midi_event_free( _apiData->coder );
    if ( _apiData->buffer ) free( _apiData->buffer );
    snd_seq_close( _apiData->seq );
    delete _apiData;
}

// This function is used to count or get the pinfo structure for a given port number.
size_t Alsa::portInfo( snd_seq_t *seq, snd_seq_port_info_t *pinfo, unsigned int type, int portNumber )
{
    snd_seq_client_info_t *cinfo;
    int client;
    size_t count = 0;
    snd_seq_client_info_alloca( &cinfo );

    snd_seq_client_info_set_client( cinfo, -1 );
    while ( snd_seq_query_next_client( seq, cinfo ) >= 0 ) {
        client = snd_seq_client_info_get_client( cinfo );
        if ( client == 0 ) continue;
        // Reset query info
        snd_seq_port_info_set_client( pinfo, client );
        snd_seq_port_info_set_port( pinfo, -1 );
        while ( snd_seq_query_next_port( seq, pinfo ) >= 0 ) {
            unsigned int atyp = snd_seq_port_info_get_type( pinfo );
            if ( ( atyp & SND_SEQ_PORT_TYPE_MIDI_GENERIC ) == 0 ) continue;
            unsigned int caps = snd_seq_port_info_get_capability( pinfo );
            if ( ( caps & type ) != type ) continue;
            if ( (int)count == portNumber ) return 1;
            ++count;
        }
    }

    // If a negative portNumber was used, return the port count.
    if ( portNumber < 0 ) return count;
    return 0;
}

size_t Alsa::getPortCount()
{
    snd_seq_port_info_t *pinfo;
    snd_seq_port_info_alloca( &pinfo );

    return portInfo(_apiData->seq, pinfo, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, -1 );
}

std::string Alsa::getPortName(unsigned int portNumber_ )
{
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;
    snd_seq_client_info_alloca( &cinfo );
    snd_seq_port_info_alloca( &pinfo );

    std::string stringName;
    if ( portInfo( _apiData->seq, pinfo, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, (int) portNumber_ ) ) {
        int cnum = snd_seq_port_info_get_client(pinfo);
        snd_seq_get_any_client_info( _apiData->seq, cnum, cinfo );
        std::ostringstream os;
        os << snd_seq_client_info_get_name(cinfo);
        os << ":";
        os << snd_seq_port_info_get_port(pinfo);
        stringName = os.str();
        return stringName;
    }

#ifndef CXXMIDI_QUIET
    std::cerr << "CxxMidi: get port name error" << std::endl;
#endif
    return stringName;
}

void Alsa::initialize()
{
    // Set up the ALSA sequencer client
    snd_seq_t *seq;
    int result = snd_seq_open( &seq, "default", SND_SEQ_OPEN_OUTPUT, SND_SEQ_NONBLOCK );
#ifndef CXXMIDI_QUIET
    if ( result < 0 )
        std::cerr << "CxxMidi: ALSA sequencer client init error" << std::endl;
#endif

    // Set client name.
    snd_seq_set_client_name( seq, "CxxMidi (RtMidi) output" );

    // Save our api-specific connection information.
    _apiData= new AlsaMidiData;
    _apiData->seq = seq;
    _apiData->vport = -1;
    _apiData->bufferSize = 32;
    _apiData->coder = 0;
    _apiData->buffer = 0;
    result = snd_midi_event_new( _apiData->bufferSize, &_apiData->coder );
    if ( result < 0 )
    {
        delete _apiData;
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: MIDI event parser init error" << std::endl;
#endif
        return;
    }
    _apiData->buffer = (unsigned char *) malloc( _apiData->bufferSize );
    if ( _apiData->buffer == NULL )
    {
        delete _apiData;
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: ALSA malloc error" << std::endl;
#endif
    }
    snd_midi_event_init( _apiData->coder );
}

void Alsa::openPort(unsigned int portNumber_)
{
    if ( _connected )
        this->closePort();

    unsigned int nSrc = this->getPortCount();
#ifndef CXXMIDI_QUIET
    if (nSrc < 1)
        std::cerr << "CxxMidi: no MIDI output sources" << std::endl;
#endif

    snd_seq_port_info_t *pinfo;
    snd_seq_port_info_alloca( &pinfo );

    if ( portInfo( _apiData->seq, pinfo, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, (int) portNumber_ ) == 0 )
    {
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: invalid port port: " << portNumber_ << std::endl;
#endif
    }

    snd_seq_addr_t sender, receiver;
    receiver.client = snd_seq_port_info_get_client( pinfo );
    receiver.port = snd_seq_port_info_get_port( pinfo );
    sender.client = snd_seq_client_id( _apiData->seq );

    if ( _apiData->vport < 0 ) {
        _apiData->vport = snd_seq_create_simple_port( _apiData->seq, "CxxMidi (RtMidi) output",
                                                      SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
                                                      SND_SEQ_PORT_TYPE_MIDI_GENERIC|SND_SEQ_PORT_TYPE_APPLICATION );
#ifndef CXXMIDI_QUIET
        if ( _apiData->vport < 0 )
            std::cerr << "CxxMidi: output port init error" << std::endl;
#endif
    }

    sender.port = _apiData->vport;

    // Make subscription
    snd_seq_port_subscribe_malloc( &_apiData->subscription );
    snd_seq_port_subscribe_set_sender(_apiData->subscription, &sender);
    snd_seq_port_subscribe_set_dest(_apiData->subscription, &receiver);
    snd_seq_port_subscribe_set_time_update(_apiData->subscription, 1);
    snd_seq_port_subscribe_set_time_real(_apiData->subscription, 1);

    if ( snd_seq_subscribe_port(_apiData->seq, _apiData->subscription) )
    {
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: ports connect error" << std::endl;
#endif
    }


    _connected = true;
}

void Alsa::closePort()
{
    if ( _connected ) {
        snd_seq_unsubscribe_port( _apiData->seq, _apiData->subscription );
        snd_seq_port_subscribe_free( _apiData->subscription );
        _connected = false;
    }
}

void Alsa::openVirtualPort( const std::string& portName_ )
{
    if ( _apiData->vport < 0 ) {
        _apiData->vport = snd_seq_create_simple_port( _apiData->seq, portName_.c_str(),
                                                      SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
                                                      SND_SEQ_PORT_TYPE_MIDI_GENERIC|SND_SEQ_PORT_TYPE_APPLICATION );
#ifndef CXXMIDI_QUIET
        if ( _apiData->vport < 0 )
            std::cerr << "CxxMidi: ALSA virtual port init error: "
                      << portName_ << std::endl;
#endif
    }
}

void Alsa::sendMessage(const std::vector<uint8_t> *msg_ )
{
    int result;
    unsigned int nBytes = msg_->size();
    if ( nBytes > _apiData->bufferSize )
    {
        _apiData->bufferSize = nBytes;
        result = snd_midi_event_resize_buffer ( _apiData->coder, nBytes);
#ifndef CXXMIDI_QUIET
        if ( result != 0 )
            std::cerr << "CxxMidi: ALSA resizee buffer error" << std::endl;
#endif
        free (_apiData->buffer);
        _apiData->buffer = (unsigned char *) malloc( _apiData->bufferSize );
        if ( _apiData->buffer == NULL )
        {
#ifndef CXXMIDI_QUIET
            std::cerr << "CxxMidi: error allocating buffer memory" << std::endl;
#endif
            return;
        }
    }

    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, _apiData->vport);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    for ( unsigned int i=0; i<nBytes; ++i ) _apiData->buffer[i] = msg_->at(i);
    result = snd_midi_event_encode( _apiData->coder, _apiData->buffer, (long)nBytes, &ev );
    if ( result < (int)nBytes )
    {
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: event parsing error" << std::endl;
#endif
        return;
    }

    // Send the event
    result = snd_seq_event_output(_apiData->seq, &ev);
#ifndef CXXMIDI_QUIET
    if ( result < 0 )
        std::cerr << "CxxMidi: error sending MIDI message to port" << std::endl;
#endif
    snd_seq_drain_output(_apiData->seq);
}

} // namespace Linux
} // namespace Output
} // namespace CxxMidi

#endif // CXXMIDI_OUTPUT_LINUX_ALSA_HPP
