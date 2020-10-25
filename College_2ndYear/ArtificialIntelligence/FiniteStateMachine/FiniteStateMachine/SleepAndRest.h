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
        std::cout << "���δ� ������ �޽��� �մϴ�!" << std::endl;

        mAttribute->Gold = 0;
        mAttribute->IsRest = true;
    }
};