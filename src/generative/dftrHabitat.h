/**
 * base class for different hunting grounds for autonomous agents
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include <string>
#include <map>
#include <memory>

namespace drifter
{
namespace generative
{
    class Fauna;

    class Habitat
    {
    protected:
        int _width;
        int _height;
        std::map<std::string, std::shared_ptr<drifter::generative::Fauna>> _faunaRefMap;

    public:
        Habitat( int width, int height ) : _width( width ), _height( height ){}

        void Initialize();
        void Draw();
    };
} //generative
} //drifter