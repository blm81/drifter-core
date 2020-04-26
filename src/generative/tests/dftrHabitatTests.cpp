/**
 * automated tests for generative::Habitat
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "dftrHabitatTests.h"
#include "dftrFaunaTester.h"
#include "generative/dftrFauna.h"
#include "generative/tests/dftrHabitatTester.h"
#include <cmath>

namespace drifter
{
using namespace generative;
/**
 * initialize the habitat for AdvanceHunt and UpdateMass tests
 * @return a vector of Fauna ids ordered from smallest to largest
 */
std::vector<std::string> HabitatTester::InitHuntingHabitat()
{
    std::vector<std::pair<float, float>> positions = {
            {50, 50},
            {20, 20},
            {75, 75}
    };
    Populate( positions );
    std::string biggestFauna = _faunaLocs[50][50]->Resident();
    std::string middleFauna = _faunaLocs[20][20]->Resident();
    std::string smallestFauna = _faunaLocs[75][75]->Resident();
    std::vector<std::shared_ptr<FaunaTester>> faunaVec;
    auto it = _faunaRefMap.find( biggestFauna );
    std::shared_ptr<FaunaTester> faunaTester = std::static_pointer_cast<FaunaTester>( it->second );
    faunaTester->SetParentRadius( 14 );
    faunaTester->SetParentColor( ci::Colorf( 1.0f, 0.0f, 0.0f ));
    faunaVec.push_back( faunaTester );
    it = _faunaRefMap.find( middleFauna );
    faunaTester = std::static_pointer_cast<FaunaTester>( it->second );
    faunaTester->SetParentRadius( 7 );
    faunaTester->SetParentColor( ci::Colorf( 0.0f, 1.0f, 0.0f ));
    faunaVec.push_back( faunaTester );
    it = _faunaRefMap.find( smallestFauna );
    faunaTester = std::static_pointer_cast<FaunaTester>( it->second );
    faunaTester->SetParentRadius( 2 );
    faunaTester->SetParentColor( ci::Colorf( 0.0f, 0.0f, 1.0f ));
    faunaVec.push_back( faunaTester );
    for ( std::shared_ptr<FaunaTester> currFaunaTester : faunaVec ) {
        currFaunaTester->MaxAge() = 100; //set to something arbitrarily large which exceeds the duration of the test
    }
    return std::vector<std::string>{ smallestFauna, middleFauna, biggestFauna };
}
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
        std::vector<std::string> faunaIds = habitat->InitHuntingHabitat();

        for ( size_t i = 0; i < 5; ++i ) {
            habitat->AdvanceHunt();
        }
        auto st = habitat->_faunaRefMap.find( faunaIds[0] );
        auto mt = habitat->_faunaRefMap.find( faunaIds[1] );
        auto bt = habitat->_faunaRefMap.find( faunaIds[2] );

        if ( st != habitat->_faunaRefMap.end() || mt == habitat->_faunaRefMap.end() || bt == habitat->_faunaRefMap.end() ) {
            std::cout << "AdvanceHuntTest failed stage 1" << std::endl;
            return false;
        }

        for ( size_t i = 0; i < 10; ++i ) {
            habitat->AdvanceHunt();
        }
        st = habitat->_faunaRefMap.find( faunaIds[0] );
        mt = habitat->_faunaRefMap.find( faunaIds[1] );
        bt = habitat->_faunaRefMap.find( faunaIds[2] );

        if ( st != habitat->_faunaRefMap.end() || mt != habitat->_faunaRefMap.end() || bt == habitat->_faunaRefMap.end() ) {
            std::cout << "AdvanceHuntTest failed stage 2" << std::endl;
            return false;
        }

        std::cout << "all AdvanceHunt tests passed!" << std::endl;
        return true;
    }

    bool UpdateMassTest()
    {
        std::function<int( const float )> prepFloat = []( const float fltIn ) -> float {
            return (int)std::floorf( fltIn * 10000 );
        };
        std::unique_ptr<HabitatTester> habitat = std::make_unique<HabitatTester>( 100, 100 );
        std::vector<std::string> faunaIds = habitat->InitHuntingHabitat();
        for ( size_t i = 0; i < 10; ++i ) {
            habitat->UpdateMass();
            auto blueIt = habitat->_massMap.find( "BLUE" );
            auto greenIt = habitat->_massMap.find( "GREEN" );
            auto redIt = habitat->_massMap.find( "RED" );
            if ( redIt == habitat->_massMap.end() || greenIt == habitat->_massMap.end() ||
                redIt == habitat->_massMap.end() ) {
                std::cout << "UpdateMassTest failed, one or more expected entries could not be found" << std::endl;
            }
            // use logging below to update test cases as the implementation changes
            /*std::cout << "UpdateMassTest iteration: " << i << ", blue mass: " << prepFloat( blueIt->second.proportionOfTotal ) <<
                      " green mass: " << prepFloat( greenIt->second.proportionOfTotal ) <<
                      " red mass: " << prepFloat( redIt->second.proportionOfTotal ) <<
                      " total mass: " << prepFloat( habitat->_totalMass ) << std::endl;*/
            switch( i ) {
                case 3:
                    if ( prepFloat( blueIt->second.proportionOfTotal ) != 869 ||
                         prepFloat( greenIt->second.proportionOfTotal ) != 3043 ||
                         prepFloat( redIt->second.proportionOfTotal ) != 6086 ||
                         prepFloat( habitat->_totalMass ) != 1445132 ) {
                        std::cout << "UpdateMassTest failed on iteration 0" << std::endl;
                        return false;
                    }
                    break;
                case 6:
                    if ( prepFloat( blueIt->second.proportionOfTotal ) != 961 ||
                         prepFloat( greenIt->second.proportionOfTotal ) != 3365 ||
                         prepFloat( redIt->second.proportionOfTotal ) != 5672 ||
                         prepFloat( habitat->_totalMass ) != 1306859 ) {
                        std::cout << "UpdateMassTest failed on iteration 6" << std::endl;
                        return false;
                    }
                    break;
                case 9:
                    if ( prepFloat( blueIt->second.proportionOfTotal ) != 1245 ||
                         prepFloat( greenIt->second.proportionOfTotal ) != 4357 ||
                         prepFloat( redIt->second.proportionOfTotal ) != 4397 ||
                         prepFloat( habitat->_totalMass ) != 1009321 ) {
                        std::cout << "UpdateMassTest failed on iteration 9" << std::endl;
                        return false;
                    }
                    break;
            }
        }
        std::cout << "all UpdateMass tests passed!" << std::endl;
        return true;
    }
} //tests
} //drifter

