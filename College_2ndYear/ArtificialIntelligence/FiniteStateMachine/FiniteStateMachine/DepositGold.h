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
        std::cout << "���δ� ���࿡�� ���� �Ա��մϴ�!" << std::endl;

        mAttribute->Gold = std::min(mAttribute->OreNum + mAttribute->Gold, mAttribute->MaxGold);
        mAttribute->OreNum = 0;

        std::cout << "���� ��� : " << mAttribute->Gold << std::endl;
    }
};
