
#include "pch.h"
#include "Point.h"

Point::Point()
    : X(0)
    , Y(0)
{
}

Point::Point(int x, int y)
    : X(x)
    , Y(y)
{
}

Point::Point(const Point& other)
    : X(other.X)
    , Y(other.Y)
{
}

Point& Point::operator=(const Point& other)
{
    X = other.X;
    Y = other.Y;
    return *this;
}

Point Point::operator+(const Point& other) const
{
    return Point(X + other.X, Y + other.Y);
}

Point& Point::operator+=(const Point& other)
{
    X += other.X;
    Y += other.Y;
    return *this;
}

Point Point::operator-(const Point& other) const
{
    return Point(X - other.X, Y - other.Y);
}

Point& Point::operator-=(const Point& other)
{
    X -= other.X;
    Y -= other.Y;
    return *this;
}

bool Point::operator==(const Point& other) const
{
    return X == other.X && Y == other.Y;
}

bool Point::operator!=(const Point& other) const
{
    return !(*this == other);
}

