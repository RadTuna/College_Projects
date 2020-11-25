
#include "Node.h"

#include <random>

Node::Node()
    : mX(0.f)
    , mY(0.f)
{
}

Node::Node(float x, float y)
    : mX(x)
    , mY(y)
{
}

void Node::SetX(float x)
{
    mX = x;
}

void Node::SetY(float y)
{
    mY = y;
}

float Node::GetX() const
{
    return mX;
}

float Node::GetY() const
{
    return mY;
}

Node Node::GetRandomElement(const Node& minElement, const Node& maxElement)
{
    static int seed = 0;

    std::random_device randomDevice;
    std::mt19937 twister(seed);
    std::uniform_real_distribution<float> distX(minElement.mX, maxElement.mX);
    std::uniform_real_distribution<float> distY(minElement.mY, maxElement.mY);

    seed += 1;
    return Node(distX(twister), distY(twister));
}


bool Node::operator==(const Node& other) const
{
    return mX == other.mX && mY == other.mY;
}

std::ostream& operator<<(std::ostream& os, const Node& node)
{
    return os << "(" << node.mX << ", " << node.mY << ")";
}
