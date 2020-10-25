#pragma once

#include "GameObject.h"

class Player final : public GameObject
{
public:
    explicit Player(class World* world, EObjectLayer collisionLayer, 
        int renderChar, unsigned short renderColor, int order);
    ~Player() override;

    void Awake() override;
    void Update() override;
    void OnCollision(GameObject* other) override;

    bool IsPowered() const;

    int GetScore() const;

private:
    void ProcessPlayerInput();

private:
    EMoveDirection mMoveDirection;

    bool mbIsPowered = false;
    int mbPowerTick = 0;

    int mScore = 0;

};
