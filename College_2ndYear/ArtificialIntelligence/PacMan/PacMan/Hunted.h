#pragma once

#include "Roam.h"
#include "Ghost.h"


class Hunted final : public Roam
{
public:
    explicit Hunted() = default;
    ~Hunted() override = default;

    void EnterState() override;
    void UpdateState() override;
    void ExitState() override;

};
