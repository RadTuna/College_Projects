
#include <iostream>
#include <cassert>

#include "StateMachine.h"

#include "MineAndDig.h"
#include "QuenchThirst.h"
#include "DepositGold.h"
#include "SleepAndRest.h"

#include "FromBankToHome.h"
#include "FromBankToMine.h"
#include "FromHomeToMine.h"
#include "FromMineToBank.h"
#include "FromMineToThirst.h"
#include "FromThirstToMine.h"

int main()
{
    enum { LOOP_LIMIT = 100 };

    StateMachine<MinerAttribute> stateMachine;
    stateMachine.GetAttribute().MaxGold = 10;
    stateMachine.GetAttribute().MaxOreNum = 5;
    stateMachine.GetAttribute().MaxWater = 2;

    const State<MinerAttribute>* mineState = stateMachine.AddState(std::make_unique<MineAndDig>());
    const State<MinerAttribute>* thirstState = stateMachine.AddState(std::make_unique<QuenchThirst>());
    const State<MinerAttribute>* bankState = stateMachine.AddState(std::make_unique<DepositGold>());
    const State<MinerAttribute>* homeState = stateMachine.AddState(std::make_unique<SleepAndRest>());

    stateMachine.SetEntryState(mineState);

    stateMachine.AddTransition(std::make_unique<FromBankToHome>(), bankState, homeState);
    stateMachine.AddTransition(std::make_unique<FromBankToMine>(), bankState, mineState);
    stateMachine.AddTransition(std::make_unique<FromHomeToMine>(), homeState, mineState);
    stateMachine.AddTransition(std::make_unique<FromMineToBank>(), mineState, bankState);
    stateMachine.AddTransition(std::make_unique<FromMineToThirst>(), mineState, thirstState);
    stateMachine.AddTransition(std::make_unique<FromThirstToMine>(), thirstState, mineState);

    for (size_t index = 0; index < LOOP_LIMIT; ++index)
    {
        stateMachine.UpdateStateMachine();
    }

    return 0;
}
