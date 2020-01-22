/**
 * base class for different hunting grounds for autonomous agents
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "generative/dftrHabitat.h"
#include "generative/dftrFauna.h"
#include "cinder/rand.h"

namespace drifter
{
namespace generative
{
    Habitat::Habitat( const int width, const int height ) : _width( width ), _height( height )
    {
        _faunaLocs.resize( height );
        for ( std::vector<std::shared_ptr<drifter::generative::HabitatTile>> & currVector : _faunaLocs ) {
            currVector.resize( width, nullptr );
        }
    }

    /**
     * initialize a randomly distributed habitat with a random number of fauna
     * TODO: replace the hardcoded numbers here, maybe pass a function here containing initialization logic?
     */
    void Habitat::Initialize()
    {
        for ( size_t i = 0; i < ci::randInt( 20, 80 ); ++i ) {
            std::shared_ptr<Fauna> fauna = std::make_shared<Fauna>( ci::vec2( ci::randFloat( 0, _width - 10 ),
                                                                              ci::randFloat( 0, _height - 10 )));
            _faunaRefMap.insert({ fauna->Id(), fauna });
            ci::vec2 currPos = fauna->Position();
            AddAtPosition( currPos.x, currPos.y, fauna->Id() );
        }
    }

    /**
     * initialize a habitat at set positions
     * @param positions: where to create the desired Fauna
     */
    void Habitat::Initialize( const std::vector<std::pair<float, float>> & positions )
    {
        for ( const std::pair<float, float> & currPos : positions ) {
            std::shared_ptr<Fauna> fauna = std::make_shared<Fauna>( ci::vec2( currPos.first, currPos.second ));
            _faunaRefMap.insert({ fauna->Id(), fauna });
            AddAtPosition( currPos.first, currPos.second, fauna->Id() );
        }
    }

    void Habitat::Draw()
    {
        for ( const std::pair<std::string, std::shared_ptr<Fauna>> & faunaPairRef : _faunaRefMap ) {
            faunaPairRef.second->Draw();
        }
    }

    /**
     * add fauna location to the habitat grid
     * @param posX: position on X axis
     * @param posY: position on Y axis
     * @param id: id of Fauna to add
     * @return -1: position is out of bounds
     */
    int16_t Habitat::AddAtPosition( const float posX, const float posY, const std::string & id )
    {
        if ( posX < 0 || posY < 0 || posX >= _width || posY >= _height ) {
            std::cout << "WARNING: agent at " << posX << ": " << posX << " cannot be added" << std::endl;
            return -1;
        }
        if ( _faunaLocs[posY][posX] == nullptr ) {
            _faunaLocs[posY][posX] = std::make_shared<HabitatTile>();
        }
        _faunaLocs[posY][posX]->SetResident( id );
        return 0;
    }
} //generative
} //drifter