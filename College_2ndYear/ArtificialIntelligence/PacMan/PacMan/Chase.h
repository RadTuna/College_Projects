#pragma once

#include "State.h"
#include "Ghost.h"


class Chase final : public State<Ghost>
{
public:
    explicit Chase() = default;
    ~Chase() override = default;

    void EnterState() override;
    void UpdateState() override;
    void ExitState() override;

};
