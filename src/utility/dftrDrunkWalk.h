/**
 * produce randomized "drunken walk" sequence of numbers
 * NOTE there's a few known brittle points here, not limited to restricting the types allowed for template
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include "cinder/Perlin.h"

namespace drifter
{
namespace utility
{
    template <class T>
    class DrunkWalk
    {
    public:

        DrunkWalk( const T minVal, const T maxVal ) : _minVal( minVal ), _maxVal( maxVal )
        {
            _prevVal = ( minVal + maxVal ) / 2; //start in the middle of the range
        }
        ~DrunkWalk(){}

        void Step( const T minStep, const T maxStep )
        {
            float noise = _perlin.noise( ci::app::getElapsedSeconds() * 0.1f ); //generate perlin noise seed
            T randVal = abs( noise ) * ( maxStep - minStep) + minStep; //produce value in range
            randVal = std::copysign( randVal, noise ); //give value the sign of the seed

            //keep values from getting stuck at the edges of range
            if ( randVal + _prevVal < _minVal || randVal + _prevVal > _maxVal )
                randVal = randVal * -1;
            _currentVal = randVal + _prevVal;

            //make sure value does not exceed bounds of range
            if (_currentVal < _minVal)
                _currentVal = _minVal;
            else if (_currentVal > _maxVal)
                _currentVal = _maxVal;

            _prevVal = _currentVal; //keep track of previous value
        }

       T CurrentValue() { return _currentVal; }

    private:

        T           _minVal;
        T           _maxVal;
        T           _prevVal;
        T           _currentVal;
        T           _minStep;
        T           _maxStep;
        ci::Perlin  _perlin;
    };
} //utility
} //drifter
