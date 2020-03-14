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
    class FaunaTester : public Fauna
    {
        friend bool tests::AdvanceHuntTest();

    public:
        FaunaTester( const ci::vec2 & position, float radius = 5 )
            : Fauna( position ) {}
    };
} //generative
} //drifter