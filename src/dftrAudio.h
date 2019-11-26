/**
 * implentation of Cinder audio API
 * NOTE: this has not been thoroughly tested post upgrade to Cinder v9.1
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include "cinder/app/App.h"
#include "cinder/audio/Context.h"
#include "cinder/audio/NodeEffects.h"
#include "cinder/audio/SamplePlayerNode.h"
#include "cinder/audio/Exception.h"
#include "cinder/audio/MonitorNode.h"

namespace drifter
{
namespace audio
{
    class AudioData
    {
    public:
        AudioData(std::string filename);
        void LoadFile(std::string filename);
        void Play() { _bufferPlayerNode->start(); }
        void SetLooped() { _bufferPlayerNode->setLoopEnabled(); }
        std::string Filename() { return _filename; }
        ci::audio::MonitorSpectralNodeRef Monitor() { return _monitorSpectralNode; }

    private:
        ci::audio::BufferPlayerNodeRef _bufferPlayerNode;
        ci::audio::GainNodeRef _gainNode;
        std::string _filename;
        ci::audio::MonitorSpectralNodeRef _monitorSpectralNode;
    };

    typedef std::shared_ptr<AudioData> AudioDataRef;
} //audio
} //drifter
