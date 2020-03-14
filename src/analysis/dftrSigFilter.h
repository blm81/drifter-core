/**
 * signal smoothing for dynamic analysis
 * NOTE there are a few known bugs here as of 11/2019
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

namespace drifter
{
namespace analysis
{
    template < class T >
    class SigFilter
    {
    public:
        SigFilter( const int sampleSize ) : _sampleSize( sampleSize ) { Init(); }

        //get sum of all weights
        T SetDenominator( std::vector< T > & weights )
        {
            T retVal = 0;
            for ( auto it = weights.begin(); it != weights.end(); ++it )
                retVal += ( *it );
            return retVal;
        }

        //returns running triangular moving average
        T TMAverage()
        {
            T totalSum = 0;
            for ( size_t i = 0; i < _input.size(); ++i ) {
                totalSum += _input[ i ] * _weights[ i ];
            }
            return totalSum / _denominator;
        }

        void Init()
        {
            T threeCoeff[] = { 1, 2, 1 };
            T fiveCoeff[] = { 1, 2, 3, 2, 1 };
            T sevenCoeff[] = { 1, 3, 6, 7, 6, 3, 1 };
            switch ( _sampleSize )
            {
                case 3:
                    _weights.assign( threeCoeff, threeCoeff + 3 );
                    break;
                case 5:
                    _weights.assign( fiveCoeff, fiveCoeff + 5 );
                    break;
                case 7:
                    _weights.assign( sevenCoeff, sevenCoeff + 7 );
                    break;
                default:
                    _sampleSize = 7;
                    _weights.assign( sevenCoeff, sevenCoeff + 7 );
                    std::cout << "sample size set to default (7)" << std::endl;
            }
            _denominator = SetDenominator( _weights );
            _middleVal = std::floor( _sampleSize / 2 ) + 1;
            _currentFilter = std::bind( &SigFilter::TMAverage, this );
        }

        void Update( const T currentInput )
        {
            if ( _input.size() < _sampleSize ) {
                _input.push_back( currentInput );
            }
            else {
                _input.erase( _input.begin() );
                _input.push_back( currentInput );
            }
        }

        //get current value
        T Calculate()
        {
            if ( _input.size() == _sampleSize )
                return _currentFilter;
            else return NULL;
        }

    private:
        size_t                      _sampleSize;
        int                         _middleVal;
        T                           _denominator;
        std::vector< T >            _weights;
        std::vector< T >            _input;
        std::function< T() >        _currentFilter; //current filter function used in Calculate
    };
} //analysis
} //drifter
