/**
 * base class for different hunting grounds for autonomous agents
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "generative/dftrHabitat.h"
#include "generative/dftrFauna.h"
#include "algorithm/dftrGraphTraversal.h"
#include "cinder/rand.h"
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
        InitMassMap({ "RED", "GREEN", "BLUE" }); //TODO implement better way to track what keys we use
        InitMassMapFunctors();
        for ( std::vector<std::shared_ptr<drifter::generative::HabitatTile>> & currVector : _faunaLocs ) {
            currVector.resize( width, nullptr );
        }
    }

    /**
     * add key if not already there, or reset to initVal
     * @param keys: keys to init/reset
     * @param initVal: value to init/reset for all keys
     */
    void Habitat::InitMassMap( const std::vector<std::string> & keys, const float initVal )
    {
        _totalMass = 0;
        for ( const std::string key : keys ) {
            auto it = _massMap.find( key );
            if ( it == _massMap.end() ) {
                _massMap.insert({ key, { key, initVal, 0.0f, [initVal]( std::shared_ptr<Fauna> fauna )-> float {
                    std::cout << "WARNING: mass update function is not set for " << fauna->Id() << ". Returning default " << initVal << std::endl;
                    return initVal;
                }}});
            }
            else {
                it->second.mass = initVal;
                it->second.proportionOfTotal = 0;
            }
        }
    }

    /**
     * match keys to std::function that increases the mass value
     * @return -1: one or more functors could not be found
     */
    int16_t Habitat::InitMassMapFunctors()
    {
        int16_t retVal = 0;
        std::function<float( const float, std::shared_ptr<Fauna>)> colorProportionFunc = [this]( const float colorVal,  std::shared_ptr<Fauna> fauna ) ->float
        {
            float proportion = colorVal / fauna->TotalColorIntensity();
            float massProport = proportion * fauna->Area();
            this->_totalMass += massProport;
            return massProport;
        };
        for ( auto it = _massMap.begin(); it != _massMap.end(); ++it ) {
            if ( it->first == "RED" ) {
                it->second.incFunc = [colorProportionFunc]( std::shared_ptr<Fauna> fauna )->float {
                    return colorProportionFunc( fauna->Color().r, fauna );
                };
            }
            else if ( it->first == "GREEN" ) {
                it->second.incFunc = [colorProportionFunc]( std::shared_ptr<Fauna> fauna )->float {
                    return colorProportionFunc( fauna->Color().g, fauna );
                };
            }
            else if ( it->first == "BLUE" ) {
                it->second.incFunc = [colorProportionFunc]( std::shared_ptr<Fauna> fauna )->float {
                    return colorProportionFunc( fauna->Color().b, fauna );
                };
            }
            else {
                std::cout << "WARNING: the key " << it->first << "does not have an increment functor" << std::endl;
                retVal = -1;
            }
        }
        return retVal;
    }

    /**
     * update the mass of the corresponding key
     * @param key: key to update
     * @param fauna:
     * @return -1: key not found
     */
    int16_t Habitat::UpdateMassEntry( const std::string & key, std::shared_ptr<Fauna> fauna )
    {
        auto it = _massMap.find( key );
        if ( it == _massMap.end() ) {
            std::cout << "WARNING: the entry for " << key << " could not be updated" << std::endl;
            return -1;
        }
        else {
            it->second.mass += it->second.incFunc( fauna );
            return 0;
        }
    }

    /**
     * update proportions for total mass
     * @return TODO only a placeholder now
     */
    int16_t Habitat::UpdateMassTotals()
    {
        float totalMass = 0;
        for ( auto it = _massMap.begin(); it != _massMap.end(); ++it ) {
            totalMass += it->second.mass;
        }
        for ( auto it = _massMap.begin(); it != _massMap.end(); ++it ) {
            it->second.proportionOfTotal = it->second.mass / _totalMass;
        }
    }

    /**
     * updates the mass entries, then totals
     */
    void Habitat::UpdateMass()
    {
        InitMassMap({ "RED", "GREEN", "BLUE" }); //TODO implement better way to track what keys we use
        for ( auto it = _faunaRefMap.begin(); it != _faunaRefMap.end(); ++it ) {
            it->second->Update();
            UpdateMassEntry( "RED", it->second );
            UpdateMassEntry( "GREEN", it->second );
            UpdateMassEntry( "BLUE", it->second );
        }
        UpdateMassTotals();
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
        UpdateMass();
        for ( const std::string & deadFaunaId : _deadFaunaIds ) {
            auto deadFaunaIt = _faunaRefMap.find( deadFaunaId );
            if ( deadFaunaIt == _faunaRefMap.end() ) {
                std::cout << "WARNING: dead fauna with id " << deadFaunaId << " could not be removed" << std::endl;
                continue;
            }
            ci::vec2 deadFaunaLoc = deadFaunaIt->second->Position();
            _faunaRefMap.erase( deadFaunaId );
            _faunaLocs[deadFaunaLoc.y][deadFaunaLoc.x];
        }
        _deadFaunaIds.clear();
    }

    void Habitat::Draw() const
    {
        static bool howManyDrawn = true;
        if ( howManyDrawn ) {
            howManyDrawn = false;
        }
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
        std::cout << "fauna with id: " << fauna->Id() << " has been initialized at " << fauna->Position().x << ": " << fauna->Position().y << std::endl;
        fauna->SetDeathCB( [&]( const std::string & idOfDead ) {
            _deadFaunaIds.push_back( idOfDead );
        });
        _faunaLocs[posY][posX]->SetResident( id );
        _faunaRefMap.insert({ fauna->Id(), fauna });
        return 0;
    }
} //generative
} //drifter