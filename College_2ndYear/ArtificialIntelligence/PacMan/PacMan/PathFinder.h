#pragma once

#include "Queue.h"
#include "Point.h"
#include "NavMap.h"
#include "TypeDefinition.h"


class PathFinder final
{
public:
    explicit PathFinder(const NavMap* navMap);
    ~PathFinder();

    Point NextPoint(const Point& from, const Point& to);
    EMoveDirection NextDirection(const Point& from, const Point& to);

private:
    void FindPath(const Point& from, const Point& to);
    void PathClear();
    bool IsNavigable(int x, int y) const;

private:
    bool** mMarkBoard;
    Point** mPathBoard;
    Queue<Point> mNavQueue;
    const NavMap* mNavMap;

};
