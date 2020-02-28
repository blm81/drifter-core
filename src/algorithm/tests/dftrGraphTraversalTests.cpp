/**
 * automated tests for graph traversal
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "dftrGraphTraversalTests.h"
#include "generative/dftrFauna.h"
#include "generative/tests/dftrHabitatTester.h"
#include "algorithm/dftrGraphTraversal.h"

namespace drifter
{
namespace tests
{
    bool GetNeighborsByLayerTest()
    {
        using namespace generative;
        using namespace algorithm;

        std::unique_ptr<HabitatTester> habitat = std::make_unique<HabitatTester>(7, 7);
        std::vector<std::pair<float, float>> positions = {
                {2, 4},
                {1, 3},
                {0, 2},
                {5, 6}
        };
        habitat->Initialize(positions);
        std::vector<std::vector<std::shared_ptr<HabitatTile>>> neighbors = GetNeighborsByLayer<std::shared_ptr<HabitatTile>>(habitat->_faunaLocs, {3, 3}, 3,
        [](std::shared_ptr<HabitatTile> tile) -> bool {
            if (tile == nullptr) {
                return false;
            }
            return !tile->IsEmpty();
        });
        ci::vec2 outPos1 = habitat->_faunaRefMap.find(neighbors[0][0]->Resident())->second->Position();
        ci::vec2 outPos2 = habitat->_faunaRefMap.find(neighbors[1][0]->Resident())->second->Position();
        ci::vec2 outPos3 = habitat->_faunaRefMap.find(neighbors[2][0]->Resident())->second->Position();
        ci::vec2 outPos4 = habitat->_faunaRefMap.find(neighbors[2][1]->Resident())->second->Position();
        if (outPos1.x != positions[0].first || outPos1.y != positions[0].second) return false;
        if (outPos2.x != positions[1].first || outPos2.y != positions[1].second) return false;
        if (outPos3.x != positions[2].first || outPos3.y != positions[2].second) return false;
        if (outPos4.x != positions[3].first || outPos4.y != positions[3].second) return false;
        std::cout << "all GetNeighborsByLayer tests passed!" << std::endl;
        return true;
    }
} //tests
} //drifter