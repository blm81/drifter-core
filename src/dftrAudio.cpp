/**
 * implentation of Cinder audio API
 * NOTE: this has not been thoroughly tested post upgrade to Cinder v9.1
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "dftrAudio.h"

namespace drifter
{
namespace audio
{
    AudioData::AudioData( std::string filename ) : _filename( filename )
    {
        try {
            auto ctx = ci::audio::Context::master();
            // create a SourceFile and set its output samplerate to match the Context.
            ci::audio::SourceFileRef sourceFile = ci::audio::load( ci::loadFile( filename ), ctx->getSampleRate() );

            // load the entire sound file into a BufferRef, and construct a BufferPlayerNode with this.
            ci::audio::BufferRef buffer = sourceFile->loadBuffer();
            _bufferPlayerNode = ctx->makeNode( new ci::audio::BufferPlayerNode( buffer ) );

            // add a Gain to reduce the volume
            _gainNode = ctx->makeNode( new ci::audio::GainNode( 1.0f ) );
            // set up "zero-padded" spectral node
            auto monitorFormat = ci::audio::MonitorSpectralNode::Format().fftSize( 2048 ).windowSize( 1024 );
            _monitorSpectralNode = ctx->makeNode( new ci::audio::MonitorSpectralNode( monitorFormat ) );

            // connect and enable the Context
            _bufferPlayerNode >> _gainNode >> _monitorSpectralNode >> ctx->getOutput();
            ctx->enable();
        }
        catch ( const ci::audio::AudioExc & exception ) { std::cout << "audio exception: " << exception.what() << std::endl; }
        catch ( const ci::audio::AudioFileExc & exception ) { std::cout << "audio exception: " << exception.what() << std::endl; }
        catch ( const ci::audio::AudioDeviceExc & exception ) { std::cout << "audio exception: " << exception.what() << std::endl; }
    }
} //audio
} //drifter