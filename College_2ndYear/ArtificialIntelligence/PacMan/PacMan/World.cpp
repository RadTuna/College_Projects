
#include "World.h"
#include "Player.h"

World::World(size_t row, size_t column)
    : mRow(row)
    , mColumn(column)
    , mNavMap(row, column)
{
}

World::~World()
{
}

bool World::DestroyGameObject(GameObject* gameObject)
{
    assert(gameObject != nullptr);

    const auto iterator = std::find_if(mGameObjects.begin(), mGameObjects.end(), 
        [gameObject](const std::unique_ptr<GameObject>& element) -> bool
        {
            return element.get() == gameObject;
        });

    if (iterator != mGameObjects.end())
    {
        mPendingGameObjects.emplace_back(iterator->get());
        iterator->get()->SetVisible(false);
        iterator->get()->Destroyed();
        return true;
    }
    else
    {
        return false;
    }
}

bool World::IsCollision(const Point& location, EObjectLayer layer)
{
    for (size_t index = 0; index < mGameObjects.size(); ++index)
    {
        GameObject* gameObject = mGameObjects[index].get();
        assert(gameObject != nullptr);

        if (gameObject->GetObjectLayer() == layer)
        {
            if (gameObject->GetLocation() == location)
            {
                return true;
            }
        }
    }

    return false;
}

bool World::IsCollisionByDirection(const Point& location, EMoveDirection direction, EObjectLayer layer)
{
    Point newLocation = location;
    switch (direction)
    {
    case EMoveDirection::Up:
        --newLocation.Y;
        break;
    case EMoveDirection::Down:
        ++newLocation.Y;
        break;
    case EMoveDirection::Left:
        --newLocation.X;
        break;
    case EMoveDirection::Right:
        ++newLocation.X;
        break;
    case EMoveDirection::Stop:
        break;
    default:
        assert(false);
        break;
    }

    return IsCollision(newLocation, layer);
}

void World::ProcessCollision(GameObject* gameObject)
{
    assert(gameObject != nullptr);

    for (size_t index = 0; index < mGameObjects.size(); ++index)
    {
        GameObject* other = mGameObjects[index].get();
        assert(other != nullptr);

        if (other != gameObject && gameObject->GetLocation() == other->GetLocation())
        {
            gameObject->OnCollision(other);
            other->OnCollision(gameObject);
        }
    }
}

void World::SetNavMap(const Point& location, ENavType navType)
{
    mNavMap.SetNavMap(location, navType);
}

const NavMap& World::GetNavMap() const
{
    return mNavMap;
}

size_t World::GetRow() const
{
    return mRow;
}

size_t World::GetColumn() const
{
    return mColumn;
}

Player* World::GetPlayer() const
{
    return mPlayer;
}

void World::Update()
{
    for (size_t index = 0; index < mGameObjects.size(); ++index)
    {
        mGameObjects[index]->Update();
    }

    ProcessPendingGameObject();

    for (size_t index = 0; index < mGameObjects.size(); ++index)
    {
        mGameObjects[index]->Render();
    }
}

void World::ProcessPendingGameObject()
{
    for (GameObject* gameObject : mPendingGameObjects)
    {
        const auto iterator = std::find_if(mGameObjects.begin(), mGameObjects.end(), 
        [gameObject](const std::unique_ptr<GameObject>& element) -> bool
        {
            return element.get() == gameObject;
        });

        assert(iterator != mGameObjects.end());
        mGameObjects.erase(iterator);
    }

    mPendingGameObjects.clear();
}
