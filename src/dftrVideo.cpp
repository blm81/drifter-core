/**
 * uses Cinder video playback API to create some interesting compositing effect
 * NOTE this is a work-in-progress with known issues
 * ALSO NOTE: this has not been thoroughly tested post upgrade to Cinder v9.1
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "dftrVideo.h"

namespace drifter
{
namespace video
{
    void VideoData::PopulateVidList( std::string vidDir )
    {
        boost::filesystem::path vidPath( vidDir );
        try {
            for ( boost::filesystem::directory_iterator iter( vidPath ); iter != boost::filesystem::directory_iterator(); ++iter ) {
                if ( iter->path().extension() == ".mov" ) {
                    _vidPaths.push_back( iter->path().string() );
                }
            }
        }
        catch ( const boost::filesystem::filesystem_error& ex ) {
            ci::app::console() << ex.what() << std::endl;
        }
    }

    void DrawToFbo( ci::gl::Fbo & fbo, std::function< void() > drawFunction )
    {
        fbo.bindFramebuffer();
        ci::gl::clear( ci::ColorAf( 0.0f, 0.0f, 0.0f, 0.0f ) );
        ci::gl::viewport( fbo.getSize() );
        ci::gl::setMatricesWindow( fbo.getSize(), false );
        drawFunction();
        fbo.unbindFramebuffer();
    }

    void DisplayFboFlipped( ci::gl::Fbo & fbo )
    {
        ci::gl::Texture2dRef displayTexture = fbo.getColorTexture();
        //TODO flip texture so displayed correctly
        ci::gl::draw( displayTexture );
    }

    bool VideoData::CheckElapsed()
    {
        if ( Elapsed( _transitionThresh, _movies[ _currentMovie ] ) && !_bIsTransitioning ) {
            _bIsTransitioning = true;
            return true;
        }
        else {
            return false;
        }
    }

    void VideoData::CueNextVideo()
    {
        _currentFile ++ ;
        if ( _currentFile >= _vidPaths.size() ) {
            _currentFile = 0;
        }
        LoadVideoFile( _currentFile, _currentMovie );
    }

    void VideoData::LoadVideoFile( const int fileIndex, const int movieIndex )
    {
        try {
            _movies[ movieIndex ]->create( _vidPaths[ fileIndex ] );
            std::cout << "playing: " << _vidPaths[ fileIndex ] << std::endl;
            _movies[ movieIndex ]->setLoop( true, true );
            _movies[ movieIndex ]->seekToStart();
        }
        catch( ... ) {
            ci::app::console() << "Unable to load the movie." << std::endl;
        }
    }

    void VideoData::SwitchLoader( const int fileIndex )
    {
        _bLoader = !_bLoader;
        LoadVideoFile( fileIndex, _bLoader );
    }

    void VideoData::SwitchPlaying()
    {
        _movies[ _bLoader ]->play();
        _movies[ _bLoader ]->setRate( 0.6f );
        _currentMovie = _bLoader;
        _movies[ !_bLoader ]->stop();
        _bIsTransitioning = false;
    }

    void VideoData::UpdateAndBind( const int glId )
    {
        //TODO the below logic is not exactly the behavior desired.  fix in a way that places videos in correct order
        //in all cases
        if (_textures.size() < 2) {
            _textures.push_back(*_movies[ _currentMovie ]->getTexture());
        }
        else {
            _textures[ _currentMovie ] = *_movies[ _currentMovie ]->getTexture();
        }
        _textures[ _currentMovie ].bind( glId );
    }

    //use grayscale image as mask for color image (CPU bound)
    ci::Surface ApplyMask( const ci::Channel & mask, const ci::Surface & rgb )
    {
        ci::Surface outputSurface( 1280, 720, true );
        ci::Channel::ConstIter maskIter = mask.getIter();
        ci::Surface::ConstIter colorIter( rgb.getIter() );
        ci::Surface::Iter targetIter( outputSurface.getIter() );
        while( maskIter.line() && colorIter.line() && targetIter.line() ) { // line by line
            while( maskIter.pixel() && colorIter.pixel() && targetIter.pixel() ) { // pixel by pixel
                float maskValue = maskIter.v() / 255.0f;
                //set rgb values in target surface
                targetIter.r() = colorIter.r();
                targetIter.g() = colorIter.g();
                targetIter.b() = colorIter.b();
                //set brigtness based on pixel value in grayscale image
                targetIter.r() *= maskValue;
                targetIter.g() *= maskValue;
                targetIter.b() *= maskValue;
            }
        }
        return outputSurface;
    }
} //video
} //drifter
