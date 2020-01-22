/**
 * algorithms for graph traversal
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include <vector>
#include <queue>
#include <utility>
#include <functional>
#include <unordered_set>
#include <boost/functional/hash.hpp>

namespace
{
    bool GridPointIsValid( const std::pair<int, int> & pointToCheck, const int maxX, const int maxY );
    void EnqueueAdjacent( const std::pair<int, int> & referencePoint, std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> & searched,
                           std::queue<std::pair<int, int>> & toSearch, const int maxX, const int maxY );
}

namespace drifter
{
namespace algorithm
{
    template <typename T>
    std::vector<std::vector<T>> GetNeighborsByLayer( const std::vector<std::vector<T>> & inputGrid, const std::pair<int, int> & referencePos,
                                                     size_t maxDepth, const std::function<bool(T)> & conditionalFunc,
                                                     bool stopIfFound = false );
} //algorithm
} //drifter