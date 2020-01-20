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
    void Habitat::Initialize()
    {
        for ( size_t i = 0; i < ci::randInt( 20, 80 ); ++i ) {
            std::shared_ptr<Fauna> fauna = std::make_shared<Fauna>( ci::vec2( ci::randFloat( 0, _width - 10 ),
                                                                              ci::randFloat( 0, _height - 10 )));
            _faunaRefMap.insert({ fauna->Id(), fauna });
        }
    }

    void Habitat::Draw()
    {
        for ( const std::pair<std::string, std::shared_ptr<Fauna>> & faunaPairRef : _faunaRefMap ) {
            faunaPairRef.second->Draw();
        }
    }
} //generative
} //drifter