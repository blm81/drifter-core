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
namespace tests
{
    bool AdvanceHuntTest()
    {
        using namespace generative;

        std::unique_ptr<HabitatTester> habitat = std::make_unique<HabitatTester>(100, 100);
        std::vector<std::pair<float, float>> positions = {
                {50, 50},
                {20, 20},
                {75, 75}
        };
        habitat->Initialize(positions);
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

