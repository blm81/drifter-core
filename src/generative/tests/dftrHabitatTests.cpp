/**
 * automated tests for generative::Habitat
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "dftrHabitatTests.h"
#include "generative/dftrFauna.h"
#include "generative/tests/dftrHabitatTester.h"

namespace drifter
{
using namespace generative;
namespace tests
{
    bool PopulateTest()
    {
        std::unique_ptr<HabitatTester> habitat = std::make_unique<HabitatTester>( 2, 2 );
        habitat->Populate(); //TODO: set population size here so we can be reasonably sure we produce a duplicate position
        if ( habitat->_faunaRefMap.size() > 4 ) {
            std::cout << "Habitat::Populate test failed with random population" << std::endl;
            return false;
        }
        habitat.release();
        habitat = nullptr;
        habitat = std::make_unique<HabitatTester>( 2, 2 );
        std::vector<std::pair<float, float>> positions = {
                {0, 0},
                {1, 1},
                {1, 1}
        };
        habitat->Populate( positions );
        if ( habitat->_faunaRefMap.size() > 2 ) {
            std::cout << "Habitat::Populate test failed with set population" << std::endl;
            return false;
        }
        std::cout << "All Populate tests passed!" << std::endl;
        return true;
    }

    bool AdvanceHuntTest()
    {
        std::unique_ptr<HabitatTester> habitat = std::make_unique<HabitatTester>( 100, 100 );
        std::vector<std::pair<float, float>> positions = {
                {50, 50},
                {20, 20},
                {75, 75}
        };
        habitat->Populate( positions );
        std::string biggestFauna = habitat->_faunaLocs[50][50]->Resident();
        std::string middleFauna = habitat->_faunaLocs[20][20]->Resident();
        std::string smallestFauna = habitat->_faunaLocs[75][75]->Resident();
        auto it = habitat->_faunaRefMap.find( biggestFauna );
        it->second->SetRadius( 21 );
        it = habitat->_faunaRefMap.find( middleFauna );
        it->second->SetRadius( 14 );
        it = habitat->_faunaRefMap.find( smallestFauna );
        it->second->SetRadius( 7 );

        for ( size_t i = 0; i < 10; ++i ) {
            habitat->AdvanceHunt();
        }
        auto st = habitat->_faunaRefMap.find( smallestFauna );
        auto mt = habitat->_faunaRefMap.find( middleFauna );
        auto bt = habitat->_faunaRefMap.find( biggestFauna );

        if ( st != habitat->_faunaRefMap.end() || mt == habitat->_faunaRefMap.end() || bt == habitat->_faunaRefMap.end() ) {
            std::cout << "AdvanceHuntTest failed stage 1" << std::endl;
            return false;
        }

        for ( size_t i = 0; i < 8; ++i ) {
            habitat->AdvanceHunt();
        }
        st = habitat->_faunaRefMap.find( smallestFauna );
        mt = habitat->_faunaRefMap.find( middleFauna );
        bt = habitat->_faunaRefMap.find( biggestFauna );

        if ( st != habitat->_faunaRefMap.end() || mt != habitat->_faunaRefMap.end() || bt == habitat->_faunaRefMap.end() ) {
            std::cout << "AdvanceHuntTest failed stage 2" << std::endl;
            return false;
        }

        std::cout << "all AdvanceHuntTest tests passed!" << std::endl;
        return true;
    }
} //tests
} //drifter

