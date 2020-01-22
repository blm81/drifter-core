/**
 * algorithms for graph traversal
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "algorithm/dftrGraphTraversal.h"
#include "generative/dftrHabitatTile.h"
#include <iostream>

namespace
{
    /**
     * determine if a point in grid is in bounds
     * @param pointToCheck: 2d point to check
     * @param maxX: grid x axis size
     * @param maxY: grid y axis size
     * @return: true if point is within bounds
     */
    bool GridPointIsValid( const std::pair<int, int> & pointToCheck, const int maxX, const int maxY )
    {
        return (pointToCheck.first >= 0 && pointToCheck.first < maxX &&
                pointToCheck.second >= 0 && pointToCheck.second < maxY );
    }

    /**
     * initialize a box search of neighbors of a 2d point, add to search queue if it has not already been searched
     * @param referencePoint: 2d point to get neighbors for
     * @param searched: set of points that have already been searched
     * @param toSearch: queue of points to search
     * @param maxX: grid x axis size
     * @param maxY: grid y axis size
     */
    void EnqueueAdjacent( const std::pair<int, int> & referencePoint, std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> & searched,
                           std::queue<std::pair<int, int>> & toSearch, const int maxX, const int maxY )
    {
        int refX = referencePoint.first;
        int refY = referencePoint.second;
        std::vector<std::function<std::pair<int, int>( int, int )>> allPositions = {
            []( int x, int y ) { return std::pair<int, int>{ x - 1, y }; },
            []( int x, int y ) { return std::pair<int, int>{ x - 1, y - 1 }; },
            []( int x, int y ) { return std::pair<int, int>{ x, y - 1 }; },
            []( int x, int y ) { return std::pair<int, int>{ x + 1, y - 1 }; },
            []( int x, int y ) { return std::pair<int, int>{ x + 1, y }; },
            []( int x, int y ) { return std::pair<int, int>{ x + 1, y + 1 }; },
            []( int x, int y ) { return std::pair<int, int>{ x, y + 1 }; },
            []( int x, int y ) { return std::pair<int, int>{ x - 1 , y + 1 }; }
        };
        for ( const std::function<std::pair<int, int>( int, int )> & currPosFunc : allPositions ) {
            std::pair<int, int> currPos = currPosFunc( referencePoint.first, referencePoint.second );
            if ( searched.count( currPos ) == 0 && GridPointIsValid( currPos, maxX, maxY )) {
                searched.insert( currPos );
                toSearch.push( currPos );
            }
        }
    }
}

namespace drifter
{
namespace algorithm
{
    /**
     * BFS in layers around a 2d point
     * this returns neighbors by layer up to a specified depth
     * @tparam T: object type at each point in grid
     * @param inputGrid: 2D grid of objects of type T
     * @param referencePoint: 2d point to get neighbors for
     * @param maxDepth: maximum search layer depth
     * @param conditionalFunc: call this to determine if a point is a neighbor
     * @param stopIfFound: stop search after finding at least one neighbor in a layer
     * @return 2D array of neighbors
     */
    template <typename T>
    std::vector<std::vector<T>> GetNeighborsByLayer( const std::vector<std::vector<T>> & inputGrid, const std::pair<int, int> & referencePoint,
                                                     size_t maxDepth, const std::function<bool(T)> & conditionalFunc,
                                                     bool stopIfFound )
    {
        std::vector<std::vector<T>> retVal;
        if ( inputGrid.empty() ) {
            std::cout << "WARNING: input to GetNeighborsByLayer is empty" << std::endl;
            return retVal;
        }
        size_t gridHeight = inputGrid.size();
        retVal.resize( gridHeight );
        size_t gridWidth = inputGrid[0].size();
        for ( const std::vector<T> & innerVec : inputGrid ) {
            if ( innerVec.size() != gridWidth ) {
                std::cout << "WARNING: input to GetNeighborsByLayer is invalid" << std::endl;
                return retVal;
            }
        }

        std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> searched;
        std::queue<std::pair<int, int>> toSearch;
        std::vector<std::pair<int, int>> nextEnqueuePass;

        /*
         * for each layer, make sure the adjacent points to points from last layer have been enqueued
         * then search the current layer for neighbors
         * exit once all layers are searched unless specified to exit after at least one neighbor is found
         * start of with the neighbors of the reference point
         */
        EnqueueAdjacent( referencePoint, searched, toSearch, gridHeight, gridWidth );
        for ( size_t layer = 0; layer < maxDepth; ++layer ) {
            bool neighborInLayer = false;
            for ( const std::pair<int, int> & toEnqueue : nextEnqueuePass ) {
                EnqueueAdjacent( toEnqueue, searched, toSearch, gridHeight, gridWidth );
            }
            nextEnqueuePass.clear();
            while (!toSearch.empty()) {
                std::pair<int, int> currPos = toSearch.front();
                nextEnqueuePass.push_back( currPos );
                T currItem = inputGrid[currPos.second][currPos.first];
                if (conditionalFunc( currItem )) {
                    neighborInLayer = true;
                    retVal[layer].push_back( currItem );
                }
                toSearch.pop();
            }
            if ( stopIfFound && neighborInLayer ) {
                break;
            }
        }
        return retVal;
    }

    template std::vector<std::vector<std::shared_ptr<drifter::generative::HabitatTile>>> GetNeighborsByLayer<std::shared_ptr<drifter::generative::HabitatTile>>(const std::vector<std::vector<std::shared_ptr<drifter::generative::HabitatTile>>> &,
                                                                                                                                                                const std::pair<int, int> &, size_t, const std::function<bool(std::shared_ptr<drifter::generative::HabitatTile>)> &, bool );
} //algorithm
} //drifter