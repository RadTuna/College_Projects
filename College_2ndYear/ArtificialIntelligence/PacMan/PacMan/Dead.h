#pragma once

#include "State.h"
#include "Ghost.h"


class Dead final : public State<Ghost>
{
public:
    explicit Dead() = default;
    ~Dead() override = default;

    void EnterState() override;
    void UpdateState() override;
    void ExitState() override;

};
