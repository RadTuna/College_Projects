#pragma once

#include "Transition.h"
#include "Ghost.h"
#include "World.h"
#include "Player.h"

class RoamToHunted final : public Transition<Ghost>
{
public:
    explicit RoamToHunted() = default;
    ~RoamToHunted() override = default;

    bool IsTransition() override;

};

inline bool RoamToHunted::IsTransition()
{
    const Player* player = mAttribute->GetWorld()->GetPlayer();
    return player->IsPowered();
}
