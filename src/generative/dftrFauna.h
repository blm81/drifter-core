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
    protected:
        std::string _id;
        ci::vec2 _position;
        ci::Colorf _color;
        float _radius;
        float _reach;
        float _maxAge;
        float _currentAge;
    public:
        Fauna( const ci::vec2 & position, float radius = 5 );
        void Draw() const;
        std::string Id() { return _id; }
        ci::vec2 Position() { return _position; }
    };
}
}