#pragma once

#include "Transition.h"
#include "MinerAttribute.h"

class FromMineToThirst final : public Transition<MinerAttribute>
{
public:
    FromMineToThirst() = default;
    ~FromMineToThirst() override = default;

    bool IsTransition() override
    {
        return mAttribute->Water == 0;
    }
};
