
#include <iostream>
#include <iomanip>
#include <sstream>

#include "AStar.h"


int main()
{
    enum { WIDTH = 10, HEIGHT = 10 };

    AStar<std::string, WIDTH, HEIGHT> gameMap(10.f);

    for (size_t i = 0; i < WIDTH; ++i)
    {
        for (size_t j = 0; j < HEIGHT; ++j)
        {
            std::stringstream stringStream;
            stringStream << "Node " << i << ", " << j;

            gameMap.SetNode(stringStream.str(), std::make_pair(i, j));
        }
    }

    // Add test obstacle
    const bool obstacles[WIDTH][HEIGHT] = {
        {false, false, false, false, false, false, false, false, false, false},
        {true, false, true, true, true, true, true, true, true, false},
        {true, false, true, false, false, false, false, false, false, false},
        {true, true, true, true, true, true, false, true, true, false},
        {true, true, false, false, false, false, false, false, true, false},
        {false, true, false, true, false, false, false, false, false, false},
        {false, true, false, false, true, true, true, true, false, true},
        {false, true, false, true, false, false, false, true, false, true},
        {false, true, false, false, false, false, false, false, false, false},
        {false, false, false, true, true, true, true, true, false, false} };

    for (size_t i = 0; i < WIDTH; ++i)
    {
        for (size_t j = 0; j < HEIGHT; ++j)
        {
            gameMap.SetObstacle(obstacles[i][j], std::make_pair(i, j));
        }
    }

    const pos_t startNode = std::make_pair(2, 1);
    const pos_t targetNode = std::make_pair(5, 0);

    gameMap.FindPath(startNode, targetNode);

    std::vector<pos_t> path;
    const bool result = gameMap.GetPath(targetNode, path);
    if (!result)
    {
        std::cout << "Do not find Path!" << std::endl;
        return 1;
    }

    std::cout << "A* Path find start!" << std::endl;
    for (const std::pair<int, int> position : path)
    {
        std::cout << gameMap.GetNode(position) << " -> ";
    }
    std::cout << "\nA* Path find end!" << std::endl;

    std::cout << "\nA* Path Visualize..." << std::endl;
    for (size_t i = 0; i < WIDTH; ++i)
    {
        for (size_t j = 0; j < HEIGHT; ++j)
        {
            const pos_t position(i, j);
            auto pathIterator = std::find(path.begin(), path.end(), position);

            if (pathIterator != path.end())
            {
                if (pathIterator == path.begin() || pathIterator == path.end() - 1)
                {
                    std::cout << "*";
                }
                else
                {
                    std::cout << "@";
                }
            }
            else if (!gameMap.IsMovable(position))
            {
                std::cout << "#";
            }
            else
            {
                std::cout << " ";
            }
        }

        std::cout << std::endl;
    }

    std::cout << "\nA* Path node weight..." << std::endl;
    for (size_t i = 0; i < WIDTH; ++i)
    {
        for (size_t j = 0; j < HEIGHT; ++j)
        {
            const pos_t position(i, j);

            const float weight = gameMap.GetWeight(position);
            if (weight == std::numeric_limits<float>::max())
            {
                std::cout << std::setw(5) << "INF";
            }
            else
            {
                std::cout << std::setw(5) << std::setprecision(3) << weight;
            }
        }

        std::cout << std::endl;
    }

    return 0;
}

