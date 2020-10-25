#pragma once

#include "Transition.h"
#include "Ghost.h"
#include "World.h"
#include "Player.h"

class ChaseToHunted final : public Transition<Ghost>
{
public:
    explicit ChaseToHunted() = default;
    ~ChaseToHunted() override = default;

    bool IsTransition() override;

};

inline bool ChaseToHunted::IsTransition()
{
    const Player* player = mAttribute->GetWorld()->GetPlayer();
    return player->IsPowered();
}

