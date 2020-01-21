/**
 * base class for quadrant at location in Habitat
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "dftrHabitatTile.h"

namespace drifter
{
namespace generative
{
    HabitatTile::HabitatTile() : _width( 1 ) //let's always default to 1 for now...
    {}

    /**
     * for now, a simple setter but in the future could accomodate more complexity
     * @param resident: id of Fauna at this tile
     */
    void HabitatTile::SetResident( std::string resident )
    {
        _resident = resident;
    }

    /**
     * for now, simply returns whether or not a Fauna id has been added to this tile
     * @return resident is default string
     */
    bool HabitatTile::IsEmpty() const
    {
        return _resident == "";
    }
} //generative
} //drifter