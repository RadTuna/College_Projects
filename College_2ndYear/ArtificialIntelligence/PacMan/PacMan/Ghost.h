#pragma once

#include "GameObject.h"
#include "StateMachine.h"
#include "PathFinder.h"


class Ghost final : public GameObject
{
public:
    explicit Ghost(class World* world, EObjectLayer collisionLayer, 
        int renderChar, unsigned short renderColor, int order);
    ~Ghost() override;

    void Awake() override;
    void Update() override;
    void OnCollision(GameObject* other) override;

    void SetMoveDirection(EMoveDirection moveDirection);
    EMoveDirection GetMoveDirection() const;

    PathFinder& GetPathFinder();

    const Point& GetStartLocation() const;

    void ResetTickCount();
    int GetTickCount() const;

    void SetDead(bool isDead);
    bool IsDead() const;

private:
    StateMachine<Ghost> mStateMachine;
    PathFinder mPathFinder;

    Point mStartLocation;
    EMoveDirection mMoveDirection = EMoveDirection::Stop;
    int mTickCount;
    bool mbIsDead;

};
