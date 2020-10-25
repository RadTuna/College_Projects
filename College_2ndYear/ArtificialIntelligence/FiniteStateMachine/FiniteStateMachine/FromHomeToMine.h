#pragma once

#include "Transition.h"
#include "MinerAttribute.h"

class FromHomeToMine final : public Transition<MinerAttribute>
{
public:
    FromHomeToMine() = default;
    ~FromHomeToMine() override = default;

    bool IsTransition() override
    {
        return mAttribute->IsRest;
    }
};
