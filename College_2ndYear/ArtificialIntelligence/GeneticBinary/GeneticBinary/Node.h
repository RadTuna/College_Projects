#pragma once

#include <iostream>

class Node final
{
public:
    Node();
    Node(float x, float y);

    void SetX(float x);
    void SetY(float y);

    float GetX() const;
    float GetY() const;

    static Node GetRandomElement(const Node& minElement, const Node& maxElement);

    bool operator==(const Node& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Node& node);

private:
    float mX;
    float mY;

};
