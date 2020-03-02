/**
 * base class for different hunting grounds for autonomous agents
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <utility>
#include "generative/dftrHabitatTile.h"

namespace drifter
{
namespace generative
{
    class Fauna;

    class Habitat
    {
    protected:
        int _width;
        int _height;
        int _popSize;
        int _popVariance;
        std::map<std::string, std::shared_ptr<drifter::generative::Fauna>> _faunaRefMap;
        std::vector<std::vector<std::shared_ptr<drifter::generative::HabitatTile>>> _faunaLocs;

        int16_t TryAddFauna( std::shared_ptr<drifter::generative::Fauna> fauna );

    public:
        Habitat( const int width, const int height );

        void Populate();
        void Populate( const std::vector<std::pair<float, float>> & positions );
        void AdvanceHunt();
        void Update();
        void Draw() const;
    };
} //generative
} //drifter