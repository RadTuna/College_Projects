#pragma once

#include <iostream>

#include "State.h"
#include "MinerAttribute.h"

class QuenchThirst final : public State<MinerAttribute>
{
public:
    QuenchThirst() = default;
    ~QuenchThirst() override = default;

    void UpdateState() override
    {
        std::cout << "���δ� ���� ���ʴϴ�!" << std::endl;

        mAttribute->Water = mAttribute->MaxWater;
    }
};
