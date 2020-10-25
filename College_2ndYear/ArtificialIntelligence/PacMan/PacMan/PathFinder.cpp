
#include "PathFinder.h"

#include <cassert>

PathFinder::PathFinder(const NavMap* navMap)
{
    assert(navMap != nullptr);
    mNavMap = navMap;

    const size_t row = mNavMap->GetRow();
    const size_t column = mNavMap->GetColumn();

    mMarkBoard = new bool* [column];
    mPathBoard = new Point * [column];
    for (int i = 0; i < column; ++i)
    {
        mMarkBoard[i] = new bool[row];
        mPathBoard[i] = new Point[row];
        for (int j = 0; j < row; ++j)
        {
            mMarkBoard[i][j] = false;
            mPathBoard[i][j] = Point(i, j);
        }
    }
}

PathFinder::~PathFinder()
{
    for (size_t i = 0; i < mNavMap->GetColumn(); ++i)
    {
        delete[] mMarkBoard[i];
        delete[] mPathBoard[i];
        mMarkBoard[i] = nullptr;
        mPathBoard[i] = nullptr;
    }

    delete[] mMarkBoard;
    delete[] mPathBoard;
    mMarkBoard = nullptr;
    mPathBoard = nullptr;
}

void PathFinder::FindPath(const Point& from, const Point& to)
{
    PathClear();

    Point currentPoint = from;
    mMarkBoard[currentPoint.X][currentPoint.Y] = true;
    mNavQueue.Enqueue(currentPoint);
    while (!mNavQueue.IsEmpty())
    {
        currentPoint = mNavQueue.GetData();
        mNavQueue.Dequeue();

        if (currentPoint == to)
        {
            return;
        }

        if (IsNavigable(currentPoint.X - 1, currentPoint.Y))
        {
            mMarkBoard[currentPoint.X - 1][currentPoint.Y] = true;
            mPathBoard[currentPoint.X - 1][currentPoint.Y] = currentPoint;
            mNavQueue.Enqueue(Point(currentPoint.X - 1, currentPoint.Y));
        }
        if (IsNavigable(currentPoint.X + 1, currentPoint.Y))
        {
            mMarkBoard[currentPoint.X + 1][currentPoint.Y] = true;
            mPathBoard[currentPoint.X + 1][currentPoint.Y] = currentPoint;
            mNavQueue.Enqueue(Point(currentPoint.X + 1, currentPoint.Y)); 
        }
        if (IsNavigable(currentPoint.X, currentPoint.Y - 1))
        {
            mMarkBoard[currentPoint.X][currentPoint.Y - 1] = true;
            mPathBoard[currentPoint.X][currentPoint.Y - 1] = currentPoint;
            mNavQueue.Enqueue(Point(currentPoint.X, currentPoint.Y - 1));
        }
        if (IsNavigable(currentPoint.X, currentPoint.Y + 1))
        {
            mMarkBoard[currentPoint.X][currentPoint.Y + 1] = true;
            mPathBoard[currentPoint.X][currentPoint.Y + 1] = currentPoint;
            mNavQueue.Enqueue(Point(currentPoint.X, currentPoint.Y + 1));
        }
    }

    assert(false);
}

Point PathFinder::NextPoint(const Point& from, const Point& to)
{
    FindPath(from, to);

    Point prevPoint = to;
    Point currentPoint = mPathBoard[to.X][to.Y];

    while (currentPoint != from)
    {
        prevPoint = currentPoint;
        currentPoint = mPathBoard[prevPoint.X][prevPoint.Y];
    }

    return prevPoint;
}

EMoveDirection PathFinder::NextDirection(const Point& from, const Point& to)
{
    const Point nextPoint = NextPoint(from, to);

    EMoveDirection outDirection = EMoveDirection::Stop;
    if (from.X == nextPoint.X)
    {
        if (from.Y > nextPoint.Y)
        {
            outDirection = EMoveDirection::Up;
        }
        else if (from.Y < nextPoint.Y)
        {
            outDirection = EMoveDirection::Down;
        }
    }
    else if (from.Y == nextPoint.Y)
    {
        if (from.X > nextPoint.X)
        {
            outDirection = EMoveDirection::Left;
        }
        else if (from.X < nextPoint.X)
        {
            outDirection = EMoveDirection::Right;
        }
    }
    else
    {
        assert(false);
    }

    return outDirection;
}

void PathFinder::PathClear()
{
    mNavQueue.Clear();

    for (int i = 0; i < mNavMap->GetColumn(); ++i)
	{
		for (int j = 0; j < mNavMap->GetRow(); ++j)
		{
			mPathBoard[i][j] = Point(i, j);
            mMarkBoard[i][j] = false;
		}
	}
}

bool PathFinder::IsNavigable(int x, int y) const
{
    if (x >= 0 && x < mNavMap->GetColumn() && y >= 0 && y < mNavMap->GetRow())
    {
        const bool isNonMarked = !mMarkBoard[x][y];
        const bool isNonObstacle = mNavMap->GetNavMap(x, y) != ENavType::Obstacle;

        return isNonMarked && isNonObstacle;
    }

    return false;
}
