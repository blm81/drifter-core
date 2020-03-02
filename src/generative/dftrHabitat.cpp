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
        _popSize = 15;
        _popVariance = 10;
        for ( std::vector<std::shared_ptr<drifter::generative::HabitatTile>> & currVector : _faunaLocs ) {
            currVector.resize( width, nullptr );
        }
    }

    /**
     * populate a randomly distributed habitat with a random number of fauna
     * TODO: replace the hardcoded numbers here
     */
    void Habitat::Populate()
    {
        int halfVariance = _popVariance / 2;
        int currPop = ci::randInt( _popSize - halfVariance, _popSize + halfVariance );
        for ( size_t i = 0; i < currPop; ++i ) {
            bool addingSuccess = false;
            int tries = 0;
            int maxTries = 5;
            while ( !addingSuccess && tries < maxTries ) {
                ci::vec2 proposedPos = ci::vec2(ci::randFloat( 0, _width - 10 ),
                                                ci::randFloat( 0, _height - 10 ));
                std::shared_ptr<Fauna> fauna = std::make_shared<Fauna>( proposedPos, ci::randFloat( 5.0f, 10.0f ));
                if ( TryAddFauna( fauna ) >= 0 ) {
                    addingSuccess = true;
                }
                ++tries;
            }
        }
    }

    /**
     * populate a habitat at set positions
     * @param positions: where to create the desired Fauna
     */
    void Habitat::Populate( const std::vector<std::pair<float, float>> & positions )
    {
        for ( const std::pair<float, float> & currPos : positions ) {
            std::shared_ptr<Fauna> fauna = std::make_shared<Fauna>( ci::vec2( currPos.first, currPos.second ));
            TryAddFauna( fauna );
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
     * add fauna location to the habitat grid and shared pointer to the ref map
     * return error code if this can't be done
     * @param fauna: shared pointer to Fauna to add
     * @return -1: position is out of bounds, -2: position is already occupied
     */
    int16_t Habitat::TryAddFauna( std::shared_ptr<drifter::generative::Fauna> fauna )
    {
        int posX = fauna->Position().x;
        int posY = fauna->Position().y;
        std::string id = fauna->Id();
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
        _faunaRefMap.insert({ fauna->Id(), fauna });
        return 0;
    }
} //generative
} //drifter