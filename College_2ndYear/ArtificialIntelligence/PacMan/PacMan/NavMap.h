#pragma once

#include "Point.h"


enum class ENavType
{
    Path,
    Obstacle
};

class NavMap final
{
public:
    explicit NavMap(size_t row, size_t column);
    ~NavMap();

    void SetNavMap(const Point& location, ENavType navType);
    ENavType GetNavMap(const Point& location) const;
    ENavType GetNavMap(int x, int y) const;

    size_t GetRow() const;
    size_t GetColumn() const;

private:
    ENavType** mNavBoard;

    size_t mRow;
    size_t mColumn;

};
