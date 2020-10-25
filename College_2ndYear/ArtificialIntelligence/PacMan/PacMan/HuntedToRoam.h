#pragma once

#include "Transition.h"
#include "Ghost.h"

class HuntedToRoam final : public Transition<Ghost>
{
public:
    explicit HuntedToRoam() = default;
    ~HuntedToRoam() override = default;

    bool IsTransition() override;

};

inline bool HuntedToRoam::IsTransition()
{
    const Player* player = mAttribute->GetWorld()->GetPlayer();
    return !player->IsPowered();
}
