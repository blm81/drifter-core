/**
 * find running min / max values of signal
 * NOTE there's a few known brittle points here, not limited to restricting the types allowed for template
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

namespace drifter
{
namespace analysis
{
    template< class T >
    class SigRange
    {
    public:
        SigRange()
        {
            _bIsInit = true;
            _range = std::make_pair< T, T >( 0, 0 );
        }

        void Update( const T currentInput )
        {
            if ( _bIsInit ) {
                _range.first = currentInput;
                _range.second = currentInput;
                _bIsInit = false;
            }
            if ( currentInput < _range.first )
                _range.first = currentInput;
            else if ( currentInput > _range.second )
                _range.second = currentInput;
        }

        T Min() { return _range.first; }
        T Max() { return _range.second; }

    private:
        std::pair< T, T >       _range; //first = min, second = max
        bool                    _bIsInit;
    };
} //analysis
} //drifter