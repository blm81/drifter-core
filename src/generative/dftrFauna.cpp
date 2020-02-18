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
        boost::uuids::uuid bId;
        _id = boost::uuids::to_string( bId );
        std::cout << "fauna with id: " << _id << " has been initialized at " << _position.x << ": " << _position.y << std::endl;
        SetRadius( radius );
        _reach = _radius; //TODO magic number
        _maxAge = ci::randFloat( 60.0f, 500.0f );
        _color = ci::Colorf( ci::randFloat( 0.0f, 1.0f ),
                ci::randFloat( 0.0f, 1.0f ),
                ci::randFloat( 0.0f, 1.0f ));
    }

    /**
     * we make the view a function of radius * a constant here, but that might not always be the desired way to do it
     */
    void Fauna::SetView()
    {
        _view = _radius * 4; //TODO magic number
    }

    void Fauna::SetRadius( const float radius )
    {
        _radius = radius;
        SetView();
    }

    /**
     * blend 1 color into another using weights
     * @param dstColor: color that will change based on srcColor and srcWeight
     * @param srcColor: color to blend into dstColor
     * @param srcWeight: how much to weigh srcColor in blending
     * @return -1: invalid weight
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
        retVal = BlendColors( _color, other->Color(), 0.6f );
        SetRadius( _radius + ( other->Radius() * 0.5 ));
        return retVal;
    }

    void Fauna::Draw() const
    {
        using namespace ci::gl;
        color( _color );
        drawSolidCircle( _position, _radius );
    }
} //generative
} //drifter