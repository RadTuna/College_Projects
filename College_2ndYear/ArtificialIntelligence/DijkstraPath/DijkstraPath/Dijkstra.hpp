#pragma once

#include "Dijkstra.h"

#include <limits>
#include <stack>
#include <cassert>


#define NODE(node, pair) node[pair.first][pair.second]


template <typename Type, size_t Width, size_t Height>
Dijkstra<Type, Width, Height>::Dijkstra()
    : mNodeWeight(1.f)
    , mDiagonalNodeWeight(std::sqrtf(2.f))
{
    for (size_t i = 0; i < Width; ++i)
    {
        for (size_t j = 0; j < Height; ++j)
        {
            mNodes[i][j] = Type();
        }
    }

    ClearPath();
}

template <typename Type, size_t Width, size_t Height>
void Dijkstra<Type, Width, Height>::FindPath(const pos_t& startNode, const pos_t& targetNode)
{
    assert(IsInBound(startNode));
    assert(IsInBound(targetNode));

    ClearPath();

    NODE(mWeights, startNode) = 0.f;
    mPriorityQueue.push(std::make_pair(0.f, startNode));

    while (!mPriorityQueue.empty())
    {
        pos_t position = mPriorityQueue.top().second;
        float weight = NODE(mWeights, position);

        mPriorityQueue.pop();

        if (NODE(mWeights, position) < weight)
        {
            continue;
        }

        NODE(mVisits, position) = true;

        std::array<pos_t, 8> adjacentNodes;
        adjacentNodes[0] = std::make_pair(position.first - 1, position.second);
        adjacentNodes[1] = std::make_pair(position.first + 1, position.second);
        adjacentNodes[2] = std::make_pair(position.first, position.second - 1);
        adjacentNodes[3] = std::make_pair(position.first, position.second + 1);
        adjacentNodes[4] = std::make_pair(position.first - 1, position.second - 1);
        adjacentNodes[5] = std::make_pair(position.first - 1, position.second + 1);
        adjacentNodes[6] = std::make_pair(position.first + 1, position.second - 1);
        adjacentNodes[7] = std::make_pair(position.first + 1, position.second + 1);

        for (const pos_t& adjacentPosition : adjacentNodes)
        {
            if (IsInBound(adjacentPosition) && IsMovable(adjacentPosition) && !NODE(mVisits, adjacentPosition))
            {
                float connectionWeight = mDiagonalNodeWeight;
                if (adjacentPosition.first == position.first || adjacentPosition.second == position.second)
                {
                    connectionWeight = mNodeWeight;
                }

                const float newWeight = weight + connectionWeight;

                if (newWeight < NODE(mWeights, adjacentPosition))
                {
                    NODE(mWeights, adjacentPosition) = newWeight;
                    mPriorityQueue.push(std::make_pair(newWeight, adjacentPosition));
                    NODE(mParents, adjacentPosition) = position;

                    if (adjacentPosition == targetNode)
                    {
                        return;
                    }
                }
            }
        }
    }
}

template <typename Type, size_t Width, size_t Height>
bool Dijkstra<Type, Width, Height>::GetPath(const pos_t& targetNode, std::vector<pos_t>& outPath) const
{
    assert(IsInBound(targetNode));

    std::stack<pos_t> reverser;

    pos_t currentNode = targetNode;

    while (currentNode != NODE(mParents, currentNode))
    {
        reverser.push(currentNode);
        currentNode = NODE(mParents, currentNode);
    }
    reverser.push(currentNode);

    while (!reverser.empty())
    {
        outPath.emplace_back(reverser.top());
        reverser.pop();
    }

    return outPath.size() > 1;
}

template <typename Type, size_t Width, size_t Height>
void Dijkstra<Type, Width, Height>::SetObstacle(const bool bIsObstacle, const pos_t& position)
{
    assert(IsInBound(position));

    NODE(mObstacleMap, position) = bIsObstacle;
}

template <typename Type, size_t Width, size_t Height>
void Dijkstra<Type, Width, Height>::SetNode(const Type& data, const pos_t& position)
{
    assert(IsInBound(position));

    NODE(mNodes, position) = data;
}

template <typename Type, size_t Width, size_t Height>
const Type& Dijkstra<Type, Width, Height>::GetNode(const pos_t& position) const
{
    assert(IsInBound(position));

    return NODE(mNodes, position);
}

template <typename Type, size_t Width, size_t Height>
float Dijkstra<Type, Width, Height>::GetWeight(const pos_t& position) const
{
    assert(IsInBound(position));

    return NODE(mWeights, position);
}

template <typename Type, size_t Width, size_t Height>
bool Dijkstra<Type, Width, Height>::IsInBound(const pos_t& position) const
{
    const bool bIsBoundWidth = position.first >= 0 && position.first < Width;
    const bool bIsBoundHeight = position.second >= 0 && position.second < Height;

    return bIsBoundWidth && bIsBoundHeight;
}

template <typename Type, size_t Width, size_t Height>
bool Dijkstra<Type, Width, Height>::IsMovable(const pos_t& position) const
{
    assert(IsInBound(position));

    return !NODE(mObstacleMap, position);
}

template <typename Type, size_t Width, size_t Height>
void Dijkstra<Type, Width, Height>::ClearPath()
{
    for (size_t i = 0; i < Width; ++i)
    {
        for (size_t j = 0; j < Height; ++j)
        {
            mVisits[i][j] = false;
            mParents[i][j] = std::make_pair(i, j);
            mWeights[i][j] = std::numeric_limits<float>::max();
        }
    }

    while (!mPriorityQueue.empty())
    {
        mPriorityQueue.pop();
    }
}

