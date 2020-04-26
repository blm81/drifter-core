/**
 * helper derived class for generative::Fauna
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include "generative/dftrFauna.h"
#include "generative/tests/dftrHabitatTests.h"

namespace drifter
{
namespace generative
{
    class HabitatTester;
    class FaunaTester : public Fauna
    {
        friend class HabitatTester;
        friend bool tests::AdvanceHuntTest();
        void SetParentColor( const ci::Colorf & dstColor ) { SetColor( dstColor );}
        void SetParentRadius( const float radius ) { SetRadius( radius );}
        float & MaxAge() { return _maxAge; }

    public:
        FaunaTester( const ci::vec2 & position, float radius = 5 )
            : Fauna( position ) {}
    };
} //generative
} //drifter