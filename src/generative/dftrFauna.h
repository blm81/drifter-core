/**
 * base class for different autonomous agents interacting within a habitat
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include "cinder/Vector.h"
#include "cinder/Color.h"

namespace drifter
{
namespace generative
{
    class Fauna
    {
    private:
        void SetView();

    protected:
        std::string _id;
        ci::vec2 _position;
        ci::Colorf _color;
        float _radius;
        float _reach;
        float _view;
        float _maxAge;
        float _currentAge;

        int16_t BlendColors( ci::Colorf & dstColor, const ci::Colorf & srcColor, const float srcWeight );

    public:
        Fauna( const ci::vec2 & position, float radius = 5 );
        void Draw() const;
        int16_t Eat( std::shared_ptr<Fauna> other );
        std::string Id() const { return _id; }
        ci::vec2 Position() const { return _position; }
        void SetPosition( const ci::vec2 & position ) { _position = position; }
        ci::Colorf Color() const { return _color; };
        float Reach() const { return _reach; }
        float Radius() const { return _radius; }
        void SetRadius( const float radius );
        float View() const { return _view; }
    };
}
}