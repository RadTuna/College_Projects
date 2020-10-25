#pragma once

#include "State.h"
#include "Ghost.h"

class Roam : public State<Ghost>
{
public:
    explicit Roam() = default;
    ~Roam() override = default;

    void EnterState() override;
    void UpdateState() override;
    void ExitState() override;

private:
    void CheckDirection(EMoveDirection direction, EObjectLayer layer);

private:
    std::vector<EMoveDirection> mMovables;

};

