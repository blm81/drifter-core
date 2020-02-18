/**
 * automated tests for generative::Habitat
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "dftrHabitatTests.h"
#include "generative/dftrFauna.h"
#include "generative/dftrHabitat.h"

namespace drifter
{
namespace tests
{
    bool AdvanceHuntTest()
    {
        using namespace generative;

        std::unique_ptr<Habitat> habitat;
        habitat = std::make_unique<Habitat>(100, 100);
        std::vector<std::pair<float, float>> positions = {
                {50, 50},
                {20, 20},
                {75, 75}
        };
        habitat->Initialize(positions);
        std::string biggestFauna = habitat->FaunaLocs()[50][50]->Resident();
        std::string middleFauna = habitat->FaunaLocs()[20][20]->Resident();
        std::string smallestFauna = habitat->FaunaLocs()[75][75]->Resident();
        auto it = habitat->FaunaRefMap().find( biggestFauna );
        it->second->SetRadius( 21 );
        it = habitat->FaunaRefMap().find( middleFauna );
        it->second->SetRadius( 14 );
        it = habitat->FaunaRefMap().find( smallestFauna );
        it->second->SetRadius( 7 );

        for ( size_t i = 0; i < 10; ++i ) {
            habitat->AdvanceHunt();
        }
        auto st = habitat->FaunaRefMap().find( smallestFauna );
        auto mt = habitat->FaunaRefMap().find( middleFauna );
        auto bt = habitat->FaunaRefMap().find( biggestFauna );

        if ( st != habitat->FaunaRefMap().end() || mt == habitat->FaunaRefMap().end() || bt == habitat->FaunaRefMap().end() ) {
            std::cout << "AdvanceHuntTest failed stage 1" << std::endl;
            return false;
        }

        for ( size_t i = 0; i < 8; ++i ) {
            habitat->AdvanceHunt();
        }
        st = habitat->FaunaRefMap().find( smallestFauna );
        mt = habitat->FaunaRefMap().find( middleFauna );
        bt = habitat->FaunaRefMap().find( biggestFauna );

        if ( st != habitat->FaunaRefMap().end() || mt != habitat->FaunaRefMap().end() || bt == habitat->FaunaRefMap().end() ) {
            std::cout << "AdvanceHuntTest failed stage 2" << std::endl;
            return false;
        }

        std::cout << "all AdvanceHuntTest tests passed!" << std::endl;
        return true;
    }
} //tests
} //drifter

