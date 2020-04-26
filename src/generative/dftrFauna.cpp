/**
 * base class for different autonomous agents interacting within a habitat
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "generative/dftrFauna.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "cinder/rand.h"
#include "cinder/gl/gl.h"

namespace drifter
{
namespace generative
{
    Fauna::Fauna( const ci::vec2 & position, float radius )
            : _currentAge( 0.0f ), _position( position )
    {
        static boost::mt19937 mtGen;
        boost::uuids::basic_random_generator<boost::mt19937> randomGen( &mtGen );
        boost::uuids::uuid bId = randomGen();
        _id = boost::uuids::to_string( bId );
        SetRadius( radius );
        _maxAge = ci::randFloat( 20.0f, 35.0f ); //TODO magic numbers
        SetColor(ci::Colorf( ci::randFloat( 0.0f, 1.0f ),
                ci::randFloat( 0.0f, 1.0f ),
                ci::randFloat( 0.0f, 1.0f )));
        _approachDeath = ci::randFloat( 0.7f, 0.15f ) * _maxAge;
        DeathCB = nullptr;
    }

    /**
     * replace the values of _color with dstColor
     * also: compute the total color intensity
     * @param dstColor: set _color to this
     */
    void Fauna::SetColor( const ci::Colorf & dstColor )
    {
        _color.r = dstColor.r;
        _color.g = dstColor.g;
        _color.b = dstColor.b;
        _totalColorIntensity = _color.r + _color.g + _color.b;
    }

    /**
     * we make the view a function of radius * a constant here, but that might not always be the desired way to do it
     */
    void Fauna::SetView()
    {
        _view = _radius * 4; //TODO magic number
    }

    /**
     * this does a lot more than just set the radius
     * it also sets the reach and the view, TODO revisit this
     * @param radius: new radius value
     */
    void Fauna::SetRadius( const float radius )
    {
        _radius = radius;
        _reach = _radius;
        _area = 2 * M_PI * radius;
        SetView();
    }

    /**
     * blend 1 color into another using weights
     * @param dstColor: color that will change based on srcColor and srcWeight
     * @param srcColor: color to blend into dstColor
     * @param srcWeight: how much to weigh srcColor in blending
     * @return -1: invalid weight
     * TODO maybe this should return a color and take an int16_t ref for error code?
     */
    int16_t Fauna::BlendColors( ci::Colorf & dstColor, const ci::Colorf & srcColor, const float srcWeight )
    {
        if ( srcWeight > 1.0f ) {
            std::cout << "WARNING: colors cannot be blended with an invalid weight of " << srcWeight << std::endl;
            return -1;
        }
        float dstWeight = 1.0f - srcWeight;
        std::function<float(const float, const float, const float, const float)> computeBlendedColor =
        []( const float srcVal, const float srcWeight, const float dstVal, const float dstWeight ) -> float {
            return ( srcVal * srcWeight ) + ( dstVal * dstWeight );
        };
        dstColor.r = computeBlendedColor( srcColor.r, srcWeight, dstColor.r, dstWeight );
        dstColor.g = computeBlendedColor( srcColor.g, srcWeight, dstColor.g, dstWeight );
        dstColor.b = computeBlendedColor( srcColor.b, srcWeight, dstColor.b, dstWeight );
        return 0;
    }

    /**
     * consume the "other" Fauna
     * @param other: the Fauna to consume
     * @return -1: see BlendColors return values
     */
    int16_t Fauna::Eat( std::shared_ptr<Fauna> other )
    {
        int retVal;
        ci::Colorf blendColor = _color;
        retVal = BlendColors( blendColor, other->Color(), 0.6f );
        SetColor( blendColor );
        SetRadius( _radius + ( other->Radius() * 0.5 ));
        return retVal;
    }

    /**
     * age the fauna and modulate whatever other params are associated with age
     * @param stepSize: how much to increment age in each update
     * @return reserved
     */
    int16_t Fauna::IncAge( const float stepSize )
    {
        _currentAge += stepSize;
        if ( _currentAge >= _maxAge ) {
            if ( DeathCB != nullptr ) {
                DeathCB( _id );
            }
            else {
                std::cout << "WARNING: Death callback for fauna " + _id + " is not set" << std::endl;
            }
        }
        else if ( _currentAge >= _approachDeath ) {
            float newRadius = _radius - ( _radius / _approachDeath );
            SetRadius( newRadius );
        }
        return 0;
    }

    void Fauna::Update()
    {
        IncAge();
    }

    void Fauna::Draw() const
    {
        using namespace ci::gl;
        color( _color );
        drawSolidCircle( _position, _radius );
    }
} //generative
} //drifter