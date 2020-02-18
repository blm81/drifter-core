/**
 * base class for different hunting grounds for autonomous agents
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "generative/dftrHabitat.h"
#include "generative/dftrFauna.h"
#include "algorithm/dftrGraphTraversal.h"
#include "cinder/rand.h"
#include <utility>
#include <math.h>
#include <tuple>

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
        for ( size_t i = 0; i < ci::randInt( 10, 20 ); ++i ) { //TODO magic numbers
            std::shared_ptr<Fauna> fauna = std::make_shared<Fauna>( ci::vec2( ci::randFloat( 0, _width - 10 ),
                                                                              ci::randFloat( 0, _height - 10 )),
                                                                              ci::randFloat( 5.0f, 10.0f ));
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

    void Habitat::AdvanceHunt()
    {
        using namespace drifter::algorithm;
        using namespace drifter::generative;

        std::vector<std::tuple<std::shared_ptr<Fauna>, std::shared_ptr<Fauna>>> seekGroups; //1) seeking 2) target

        for ( auto it = _faunaRefMap.begin(); it != _faunaRefMap.end(); ++it ) {
            //find the neighbors
            std::shared_ptr<Fauna> currFauna = it->second;
            ci::vec2 currPosVec = currFauna->Position();
            std::pair<int, int> currPos = std::pair<int, int>{ currPosVec.x, currPosVec.y };
            size_t currReach = trunc( currFauna->View() );
            std::vector<std::vector<std::shared_ptr<HabitatTile>>> neighbors = GetNeighborsByLayer<std::shared_ptr<HabitatTile>>( _faunaLocs, currPos, currReach,
            [] ( std::shared_ptr<HabitatTile> tile ) -> bool {
                if ( tile == nullptr ) {
                    return false;
                }
                return !tile->IsEmpty();
            });

            //size the neighbors up
            std::shared_ptr<Fauna> seekTarget = nullptr;
            for ( size_t i = 0; i < neighbors.size(); ++i ) {
                bool foundTarget = false;
                for ( size_t j = 0; j < neighbors[i].size(); ++j ) {
                    std::string currTargetId = neighbors[i][j]->Resident();
                    auto faunaIt = _faunaRefMap.find( currTargetId );
                    if ( faunaIt == _faunaRefMap.end() ) {
                        continue;
                    }
                    if ( currFauna->Radius() > faunaIt->second->Radius() ) {
                        seekGroups.push_back({ currFauna, faunaIt->second });
                        foundTarget = true;
                        break;
                    }
                }
                if ( foundTarget ) break;
            }
        }

        //seek to the closest smaller neighbor, if overtaken bigger eats smaller
        for ( size_t i = 0; i < seekGroups.size(); ++i ) {
            std::shared_ptr<Fauna> seeker = std::get<0>( seekGroups[i] );
            std::shared_ptr<Fauna> target = std::get<1>( seekGroups[i] );
            float distToTarget = ci::distance( seeker->Position(), target->Position() );
            ci::vec2 newSeekerPos;
            //seek the smaller
            if ( distToTarget > seeker->Reach() ) {
                ci::vec2 movePos = glm::normalize( seeker->Position() - target->Position() ) * seeker->Reach();
                newSeekerPos = seeker->Position() - movePos;
            }
            //overtake and eat the smaller
            else {
                newSeekerPos = target->Position();
                seeker->Eat( target );
                _faunaRefMap.erase( target->Id() );
            }
            ci::clamp( newSeekerPos.x, 0.0f,  (float)(_width - 1));
            ci::clamp( newSeekerPos.y, 0.0f,  (float)(_height - 1));
            seeker->SetPosition( newSeekerPos );
            std::shared_ptr<HabitatTile> oldSeekTile = _faunaLocs[seeker->Position().y][seeker->Position().x];
            std::shared_ptr<HabitatTile> newSeekTile = _faunaLocs[newSeekerPos.y][newSeekerPos.x];
            if ( newSeekTile == nullptr ) {
                newSeekTile = std::make_shared<HabitatTile>();
            }
            newSeekTile->SetResident( seeker->Id() );
        }
    }

    void Habitat::Update()
    {
        AdvanceHunt();
    }

    void Habitat::Draw() const
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
     * @return -1: position is out of bounds, -2: position is already occupied
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
        else {
            std::cout << "fauna at x: " << posX << " y: " << posY << " could not be added" << std::endl;
            return -2;
        }
        _faunaLocs[posY][posX]->SetResident( id );
        return 0;
    }
} //generative
} //drifter