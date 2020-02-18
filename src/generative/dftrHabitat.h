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
        std::map<std::string, std::shared_ptr<drifter::generative::Fauna>> _faunaRefMap;
        std::vector<std::vector<std::shared_ptr<drifter::generative::HabitatTile>>> _faunaLocs;

        int16_t AddAtPosition( const float posX, const float posY, const std::string & id );

    public:
        Habitat( const int width, const int height );

        void Initialize();
        void Initialize( const std::vector<std::pair<float, float>> & positions );
        void AdvanceHunt();
        void Update();
        void Draw() const;

        //TEMP ACCESSORS
        std::map<std::string, std::shared_ptr<drifter::generative::Fauna>> & FaunaRefMap() { return _faunaRefMap; };
        std::vector<std::vector<std::shared_ptr<drifter::generative::HabitatTile>>> FaunaLocs() { return _faunaLocs; }
    };
} //generative
} //drifter