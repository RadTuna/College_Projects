#pragma once

#include <vector>
#include <utility>
#include <queue>
#include <array>


typedef std::pair<int, int> pos_t;

template<typename Type, size_t Width, size_t Height>
class AStar final
{
public:
    explicit AStar(float heuristicMultiplier = 1.f);
    ~AStar() = default;

    void FindPath(const pos_t& startNode, const pos_t& targetNode);
    bool GetPath(const pos_t& targetNode, std::vector<pos_t>& outPath) const;

    void SetObstacle(const bool bIsObstacle, const pos_t& position);
    void SetNode(const Type& data, const pos_t& position);
    const Type& GetNode(const pos_t& position) const;
    float GetWeight(const pos_t& position) const;

    bool IsInBound(const pos_t& position) const;
    bool IsMovable(const pos_t& position) const;

private:
    void ClearPath();
    float GetEuclidHeuristic(const pos_t& start, const pos_t& target);

private:
    std::array<std::array<Type, Height>, Width> mNodes;
    std::array<std::array<bool, Height>, Width> mVisits;
    std::array<std::array<pos_t, Height>, Width> mParents;
    std::array<std::array<float, Height>, Width> mWeights;
    std::array<std::array<bool, Height>, Width> mObstacleMap;

    typedef std::pair<float, pos_t> queue_t;
    std::priority_queue<queue_t, std::vector<queue_t>, std::greater<queue_t>> mPriorityQueue;

    float mNodeWeight;
    float mDiagonalNodeWeight;
    float mHeuristicMultiplier;

};

#include "AStar.hpp"

