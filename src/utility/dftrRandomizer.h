/**
 * produce a series of non-repeating integers
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include <algorithm> 
#include <vector>
#include <random>

namespace drifter
{
namespace utility
{
    class Randomizer
    {
    public:
        Randomizer( const int sequenceSize ) : _sequenceSize( sequenceSize ), _currentIndex( 0 )
        {
            ScrambleIndices( sequenceSize );
        }

        //get the value of current index in scrambled vector
        int CurrentValue()
        { return _scrambled[ _currentIndex ]; };

        //advance the current index in scrambled vector
        void Advance()
        {
            _currentIndex++;
            if ( _currentIndex >= _sequenceSize ) {
                _currentIndex = 0;
                ScrambleIndices( _sequenceSize );
            }
        };

    private:
        int _currentIndex;
        int _sequenceSize;
        std::vector<int> _scrambled;

        void ScrambleIndices( const int totalIndices )
        {
            try {
                if ( _scrambled.size() > 0 ) _scrambled.clear();
                for ( int i = 0; i < _sequenceSize; ++i ) _scrambled.push_back( i );
                //use clock time to seed shuffle
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::shuffle( _scrambled.begin(), _scrambled.end(), std::default_random_engine( seed ));
            }
            catch ( const std::exception & exception ) { std::cout << exception.what() << std::endl; };
        }
    };

    typedef std::shared_ptr<Randomizer> RandomizerRef;
} //utility
} //drifter
