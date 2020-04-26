/**
 * helper derived class for generative::Habitat
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include "generative/dftrHabitat.h"
#include "generative/tests/dftrHabitatTests.h"
#include "algorithm/tests/dftrGraphTraversalTests.h"

namespace drifter
{
namespace generative
{
    class HabitatTester : public Habitat
    {
        std::vector<std::string> InitHuntingHabitat();
        friend bool tests::PopulateTest();
        friend bool tests::AdvanceHuntTest();
        friend bool tests::UpdateMassTest();
        friend bool tests::GetNeighborsByLayerTest();

    public:
        HabitatTester( const int width, const int height ) : Habitat( width, height ) {}
    };
} //generative
} //drifter