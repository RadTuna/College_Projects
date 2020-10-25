#pragma once

#include <iostream>

#include "State.h"
#include "MinerAttribute.h"

class SleepAndRest final : public State<MinerAttribute>
{
public:
    SleepAndRest() = default;
    ~SleepAndRest() override = default;

    void UpdateState() override
    {
        std::cout << "광부는 집에서 휴식을 합니다!" << std::endl;

        mAttribute->Gold = 0;
        mAttribute->IsRest = true;
    }
};