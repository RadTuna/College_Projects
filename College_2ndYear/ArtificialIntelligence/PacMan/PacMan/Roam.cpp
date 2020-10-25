
#include "Roam.h"

#include <random>
#include <iterator>

#include "World.h"


void Roam::EnterState()
{
    mAttribute->ResetTickCount();
}

void Roam::UpdateState()
{
    mMovables.clear();

    switch (mAttribute->GetMoveDirection())
    {
    case EMoveDirection::Up:
        CheckDirection(EMoveDirection::Up, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Left, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Right, EObjectLayer::Wall);
        break;
    case EMoveDirection::Down:
        CheckDirection(EMoveDirection::Down, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Left, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Right, EObjectLayer::Wall);
        break;
    case EMoveDirection::Left:
        CheckDirection(EMoveDirection::Up, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Down, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Left, EObjectLayer::Wall);
        break;
    case EMoveDirection::Right:
        CheckDirection(EMoveDirection::Up, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Down, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Right, EObjectLayer::Wall);
        break;
    case EMoveDirection::Stop:
        CheckDirection(EMoveDirection::Up, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Down, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Left, EObjectLayer::Wall);
        CheckDirection(EMoveDirection::Right, EObjectLayer::Wall);
        break;
    default:
        assert(false);
        break;
    }

    if (!mMovables.empty())
    {
        std::random_device randDev;
        std::uniform_int_distribution<size_t> dist(0, mMovables.size() - 1);

        mAttribute->SetMoveDirection(mMovables[dist(randDev)]);
    }
    else
    {
        switch (mAttribute->GetMoveDirection())
        {
        case EMoveDirection::Up:
            mAttribute->SetMoveDirection(EMoveDirection::Down);
            break;
        case EMoveDirection::Down:
            mAttribute->SetMoveDirection(EMoveDirection::Up);
            break;
        case EMoveDirection::Left:
            mAttribute->SetMoveDirection(EMoveDirection::Right);
            break;
        case EMoveDirection::Right:
            mAttribute->SetMoveDirection(EMoveDirection::Left);
            break;
        default:
            assert(false);
            break;
        }
    }
}

void Roam::ExitState()
{
}

void Roam::CheckDirection(EMoveDirection direction, EObjectLayer layer)
{
    World* world = mAttribute->GetWorld();
    assert(world != nullptr);

    const Point& location = mAttribute->GetLocation();

    if (!world->IsCollisionByDirection(location, direction, layer))
    {
        mMovables.emplace_back(direction);
    }
}
