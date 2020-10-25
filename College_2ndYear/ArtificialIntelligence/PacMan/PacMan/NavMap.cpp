
#include "NavMap.h"


NavMap::NavMap(size_t row, size_t column)
    : mRow(row)
    , mColumn(column)
{
    mNavBoard = new ENavType * [mColumn];
    for (size_t i = 0; i < mColumn; ++i)
    {
        mNavBoard[i] = new ENavType[mRow];
        for (size_t j = 0; j < mRow; ++j)
        {
            mNavBoard[i][j] = ENavType::Path;
        }
    }
}

NavMap::~NavMap()
{
    for (size_t i = 0; i < mColumn; ++i)
    {
        delete[] mNavBoard[i];
        mNavBoard[i] = nullptr;
    }

    delete[] mNavBoard;
    mNavBoard = nullptr;
}

void NavMap::SetNavMap(const Point& location, ENavType navType)
{
    mNavBoard[location.X][location.Y] = navType;
}

ENavType NavMap::GetNavMap(const Point& location) const
{
    return GetNavMap(location.X, location.Y);
}

ENavType NavMap::GetNavMap(int x, int y) const
{
    return mNavBoard[x][y];
}

size_t NavMap::GetRow() const
{
    return mRow;
}

size_t NavMap::GetColumn() const
{
    return mColumn;
}
