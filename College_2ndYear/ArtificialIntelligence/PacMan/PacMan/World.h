#pragma once

#include <vector>
#include <memory>
#include <cassert>

#include "GameObject.h"
#include "NavMap.h"
#include "TypeDefinition.h"

class World final
{
public:
    explicit World(size_t row, size_t column);
    ~World();

    template<typename Type, typename... Args>
    void SpawnGameObject(const Point& spawnLocation, Args... initArgs);
    bool DestroyGameObject(GameObject* gameObject);

    bool IsCollision(const Point& location, EObjectLayer layer);
    bool IsCollisionByDirection(const Point& location, EMoveDirection direction, EObjectLayer layer);
    void ProcessCollision(GameObject* gameObject);

    void SetNavMap(const Point& location, ENavType navType);
    const NavMap& GetNavMap() const;

    size_t GetRow() const;
    size_t GetColumn() const;

    class Player* GetPlayer() const;

    void Update();

private:
    void ProcessPendingGameObject();

private:
    std::vector<std::unique_ptr<GameObject>> mGameObjects;
    std::vector<GameObject*> mPendingGameObjects;
    NavMap mNavMap;

    class Player* mPlayer = nullptr;

    size_t mRow;
    size_t mColumn;

};

template <typename Type, typename ... Args>
void World::SpawnGameObject(const Point& spawnLocation, Args... initArgs)
{
    static_assert(std::is_base_of<GameObject, Type>::value);

    assert(spawnLocation.X < mColumn);
    assert(spawnLocation.Y < mRow);

    std::unique_ptr<GameObject> newGameObject = std::make_unique<Type>(this, initArgs...);
    newGameObject->SetLocation(spawnLocation);
    newGameObject->Awake();

    if (newGameObject->GetObjectLayer() == EObjectLayer::Player)
    {
        assert(mPlayer == nullptr);
        mPlayer = reinterpret_cast<Player*>(newGameObject.get());
    }

    mGameObjects.emplace_back(std::move(newGameObject));
}
