#pragma once

#include <iostream>

#include "State.h"
#include "MinerAttribute.h"

class MineAndDig final : public State<MinerAttribute>
{
public:
    MineAndDig() = default;
    ~MineAndDig() override = default;

    void UpdateState() override
    {
        std::cout << "���δ� ���꿡�� ������ ĺ���ϴ�!" << std::endl;

        mAttribute->OreNum = std::min(mAttribute->OreNum + 1, mAttribute->MaxOreNum);
        mAttribute->IsRest = false;
        mAttribute->Water = std::max(mAttribute->Water - 1, 0);
    }
};
