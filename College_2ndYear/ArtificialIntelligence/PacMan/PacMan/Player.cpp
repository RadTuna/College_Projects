
#include "Player.h"

#include "InputManager.h"
#include "PacMan.h"
#include "World.h"


Player::Player(World* world, EObjectLayer collisionLayer, 
    int renderChar, unsigned short renderColor, int order)
    : GameObject(world, collisionLayer, renderChar, renderColor, order)
{
}

Player::~Player()
{
}

void Player::Awake()
{
    GameObject::Awake();
}

void Player::Update()
{
    GameObject::Update();

    ProcessPlayerInput();

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

    if (mbIsPowered)
    {
        ++mbPowerTick;

        if (mbPowerTick > 30)
        {
            mbPowerTick = 0;
            mbIsPowered = false;
        }
    }
}

void Player::OnCollision(GameObject* other)
{
    GameObject::OnCollision(other);

    assert(other != nullptr);

    if (other->GetObjectLayer() == EObjectLayer::Pill)
    {
        other->Destroy();
        mScore += 10;
    }
    else if (other->GetObjectLayer() == EObjectLayer::PowerPill)
    {
        other->Destroy();
        mbIsPowered = true;
        mbPowerTick = 0;
    }
    else if (other->GetObjectLayer() == EObjectLayer::Ghost)
    {
        if (mbIsPowered)
        {
            mScore += 100;
        }
        else
        {
            PacMan::GetInstance()->GameEnd();
        }
    }
}

bool Player::IsPowered() const
{
    return mbIsPowered;
}

int Player::GetScore() const
{
    return mScore;
}

void Player::ProcessPlayerInput()
{
    if (InputManager::IsPressed(Key::Up))
    {
        mMoveDirection = EMoveDirection::Up;
    }
    else if (InputManager::IsPressed(Key::Down))
    {
        mMoveDirection = EMoveDirection::Down;
    }
    else if (InputManager::IsPressed(Key::Left))
    {
        mMoveDirection = EMoveDirection::Left;
    }
    else if (InputManager::IsPressed(Key::Right))
    {
        mMoveDirection = EMoveDirection::Right;
    }
    else if (InputManager::IsPressed(Key::End))
    {
        PacMan::GetInstance()->GameEnd();
        return;
    }

    World* world = GetWorld();
    assert(world != nullptr);

    if (world->IsCollisionByDirection(GetLocation(), mMoveDirection, EObjectLayer::Wall))
    {
        mMoveDirection = EMoveDirection::Stop;
    }
}
