/**
 * base class for different autonomous agents interacting within a habitat
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include "cinder/Vector.h"
#include "cinder/Color.h"
#include <functional>

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
        float _approachDeath; //value between 0 and 1.0, max age * this value = beginning of decline into death

        std::function<void( const std::string & )> DeathCB;
        int16_t BlendColors( ci::Colorf & dstColor, const ci::Colorf & srcColor, const float srcWeight );
        int16_t IncAge( const float stepSize = 1.0f );

    public:
        Fauna( const ci::vec2 & position, float radius = 5 );
        void Update();
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
        void SetDeathCB( const std::function<void( const std::string & )> deathCB ) { DeathCB = deathCB; }
    };
}
}