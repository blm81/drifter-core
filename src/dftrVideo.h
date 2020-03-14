/**
 * uses Cinder video playback API to create some interesting compositing effect
 * NOTE this is a work-in-progress with known issues
 * ALSO NOTE: this has not been thoroughly tested post upgrade to Cinder v9.1
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include "cinder/app/AppBase.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Surface.h"
#include "cinder/qtime/QuickTimeGlImplAvf.h"
#include "cinder/gl/Texture.h"
#include "boost/filesystem.hpp"
#include "utility/dftrRandomizer.h"

namespace drifter
{
namespace video
{
    class VideoData
    {
    private:
        std::vector <ci::qtime::MovieGlRef>  _movies;
        std::vector <ci::gl::Texture>       _textures;
        std::vector <std::string>           _vidPaths;
        bool                                _bLoader;
        bool                                _bIsTransitioning;
        int                                 _currentFile;           //current index in file list
        int                                 _currentMovie;          //current index in movie/texture lists
        float                               _transitionThresh;      //percentage elapsed for transition

    public:
        VideoData()
        {
            _movies.resize( 2 );
            _currentFile = 0;
            _bLoader = 0;
            _currentMovie = 0;
            _bIsTransitioning = false;
            _transitionThresh = 0.95f;
        }

        void                              PopulateVidList( std::string vidDir );
        void                              CueNextVideo();
        bool                              CheckElapsed();
        void                              LoadVideoFile( const int fileIndex, const int movieIndex );
        void                              SwitchLoader( const int fileIndex );
        void                              SwitchPlaying();
        void                              UpdateAndBind( const int glId );

        //accessors
        std::vector <ci::qtime::MovieGlRef>   & Movies()                   { return _movies; }
        std::vector <ci::gl::Texture>      & Textures()                 { return _textures; }
        std::vector <std::string>          & VidPaths()                 { return _vidPaths; }
        int                               CurrentFile()                   { return _currentFile; }
        int                               CurrentMovie()                  { return _currentMovie; }
        void                              ToggleLoader()                  { _bLoader = !_bLoader; }
        bool                              IsTransitioning()               { return _bIsTransitioning; }
        void                              SetIsTransitioning( const bool isTransitioning )
            { _bIsTransitioning = isTransitioning; }
    };

    ci::Surface ApplyMask( const ci::Channel & mask, const ci::Surface & rgb );
    void DrawToFbo( ci::gl::Fbo & fbo, std::function< void() > drawFunction );
    void DisplayFboFlipped( ci::gl::Fbo & fbo );

    inline float PercentElapsed( const ci::qtime::MovieGlRef movieGl )
    {
        if ( movieGl->isPlaying() )
            return movieGl->getCurrentTime() / movieGl->getDuration();
        else return 0.0f;
    }

    inline bool Elapsed( const float threshold, const ci::qtime::MovieGlRef movieGl )
    {
        if ( PercentElapsed( movieGl ) >= threshold )
            return true;
        else return false;
    }
} //video
} //drifter
