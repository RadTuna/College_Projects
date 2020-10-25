#pragma once

#include <iostream>

#include "State.h"
#include "MinerAttribute.h"

class DepositGold final : public State<MinerAttribute>
{
public:
    DepositGold() = default;
    ~DepositGold() override = default;

    void UpdateState() override
    {
        std::cout << "광부는 은행에서 돈을 입금합니다!" << std::endl;

        mAttribute->Gold = std::min(mAttribute->OreNum + mAttribute->Gold, mAttribute->MaxGold);
        mAttribute->OreNum = 0;

        std::cout << "현재 골드 : " << mAttribute->Gold << std::endl;
    }
};
