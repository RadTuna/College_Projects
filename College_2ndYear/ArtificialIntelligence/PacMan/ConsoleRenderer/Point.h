#pragma once

struct Point final
{
public:
    explicit Point();
    explicit Point(int x, int y);
    ~Point() = default;

    Point(const Point& other);

    Point& operator=(const Point& other);
    Point operator+(const Point& other) const;
    Point& operator+=(const Point& other);
    Point operator-(const Point& other) const;
    Point& operator-=(const Point& other);
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;

public:
    int X;
    int Y;

};