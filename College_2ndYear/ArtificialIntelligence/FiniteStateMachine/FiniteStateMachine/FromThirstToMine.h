#pragma once

#include "Transition.h"
#include "MinerAttribute.h"

class FromThirstToMine final : public Transition<MinerAttribute>
{
public:
    FromThirstToMine() = default;
    ~FromThirstToMine() override = default;

    bool IsTransition() override
    {
        return mAttribute->Water == mAttribute->MaxWater;
    }
};
