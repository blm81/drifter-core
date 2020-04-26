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
        struct MassEntity {
            std::string name;
            float mass;
            float proportionOfTotal;
            std::function<float( std::shared_ptr<Fauna> )> incFunc;
        };

        int _width;
        int _height;
        int _popSize;
        int _popVariance;
        float _totalMass;
        std::map<std::string, std::shared_ptr<drifter::generative::Fauna>> _faunaRefMap;
        std::map<std::string, MassEntity> _massMap; //keep track of how much mass in habitat per color, fauna type, etc
        std::vector<std::vector<std::shared_ptr<drifter::generative::HabitatTile>>> _faunaLocs;
        std::vector<std::string> _deadFaunaIds;

        void InitMassMap( const std::vector<std::string> & keys, const float initVal = 0.0f );
        int16_t InitMassMapFunctors();
        int16_t UpdateMassEntry( const std::string & key, std::shared_ptr<Fauna> fauna );
        int16_t UpdateMassTotals();
        int16_t TryAddFauna( std::shared_ptr<drifter::generative::Fauna> fauna );
        void AdvanceHunt();
        void UpdateMass();

    public:
        Habitat( const int width, const int height );

        void Populate();
        void Populate( const std::vector<std::pair<float, float>> & positions );
        void Update();
        void Draw() const;
    };
} //generative
} //drifter