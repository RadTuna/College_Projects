
#include "Ghost.h"
#include "World.h"

// States
#include "Chase.h"
#include "Dead.h"
#include "Hunted.h"
#include "Roam.h"

// Transitions
#include "ChaseToHunted.h"
#include "ChaseToRoam.h"
#include "DeadToRoam.h"
#include "HuntedToDead.h"
#include "HuntedToRoam.h"
#include "RoamToChase.h"
#include "RoamToHunted.h"


Ghost::Ghost(World* world, EObjectLayer collisionLayer, 
    int renderChar, unsigned short renderColor, int order)
    : GameObject(world, collisionLayer, renderChar, renderColor, order)
    , mStateMachine(this)
    , mPathFinder(&GetWorld()->GetNavMap())
{
    const State<Ghost>* chase = mStateMachine.AddState(std::make_unique<Chase>());
    const State<Ghost>* dead = mStateMachine.AddState(std::make_unique<Dead>());
    const State<Ghost>* hunted = mStateMachine.AddState(std::make_unique<Hunted>());
    const State<Ghost>* roam = mStateMachine.AddState(std::make_unique<Roam>());

    mStateMachine.AddTransition(std::make_unique<ChaseToHunted>(), chase, hunted);
    mStateMachine.AddTransition(std::make_unique<ChaseToRoam>(), chase, roam);
    mStateMachine.AddTransition(std::make_unique<DeadToRoam>(), dead, roam);
    mStateMachine.AddTransition(std::make_unique<HuntedToDead>(), hunted, dead);
    mStateMachine.AddTransition(std::make_unique<HuntedToRoam>(), hunted, roam);
    mStateMachine.AddTransition(std::make_unique<RoamToChase>(), roam, chase);
    mStateMachine.AddTransition(std::make_unique<RoamToHunted>(), roam, hunted);

    mStateMachine.SetEntryState(roam);
}

Ghost::~Ghost()
{
}

void Ghost::Awake()
{
    GameObject::Awake();

    mStartLocation = GetLocation();
}

void Ghost::Update()
{
    GameObject::Update();

    ++mTickCount;
    mStateMachine.UpdateStateMachine();

    switch (mMoveDirection)
    {
    case EMoveDirection::Up:
        AddLocation(Point(0, -1));
        break;
    case EMoveDirection::Down:
        AddLocation(Point(0, 1));
        break;
    case EMoveDirection::Left:
        AddLocation(Point(-1, 0));
        break;
    case EMoveDirection::Right:
        AddLocation(Point(1, 0));
        break;
    case EMoveDirection::Stop:
        break;
    default:
        assert(false);
        break;
    }
}

void Ghost::OnCollision(GameObject* other)
{
    assert(other != nullptr);

    if (other->GetObjectLayer() == EObjectLayer::Player)
    {
        SetDead(true);
    }
}

void Ghost::SetMoveDirection(EMoveDirection moveDirection)
{
    mMoveDirection = moveDirection;
}

EMoveDirection Ghost::GetMoveDirection() const
{
    return mMoveDirection;
}

PathFinder& Ghost::GetPathFinder()
{
    return mPathFinder;
}

const Point& Ghost::GetStartLocation() const
{
    return mStartLocation;
}

void Ghost::ResetTickCount()
{
    mTickCount = 0;
}

int Ghost::GetTickCount() const
{
    return mTickCount;
}

void Ghost::SetDead(bool isDead)
{
    mbIsDead = isDead;
}

bool Ghost::IsDead() const
{
    return mbIsDead;
}
