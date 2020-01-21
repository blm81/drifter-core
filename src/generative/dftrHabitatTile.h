/**
 * base class for quadrant at location in Habitat
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include <string>

namespace drifter
{
namespace generative
{
    class HabitatTile
    {
        int _width; //all tiles are assumed to be square so height is the same as this value
        std::string _resident;

    public:
        HabitatTile();

        void SetResident( std::string resident );
        bool IsEmpty() const;
    };
} //generative
} //drifter