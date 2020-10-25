
#include "Chase.h"

#include "World.h"
#include "Player.h"

void Chase::EnterState()
{
    mAttribute->ResetTickCount();
}

void Chase::UpdateState()
{
    PathFinder& pathFinder = mAttribute->GetPathFinder();

    const Point& myLocation = mAttribute->GetLocation();
    const Point& targetLocation = mAttribute->GetWorld()->GetPlayer()->GetLocation();

    mAttribute->SetMoveDirection(pathFinder.NextDirection(myLocation, targetLocation));
}

void Chase::ExitState()
{
}
