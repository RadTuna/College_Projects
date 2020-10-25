#pragma once

#include "Transition.h"
#include "Ghost.h"

class ChaseToRoam final : public Transition<Ghost>
{
public:
    explicit ChaseToRoam() = default;
    ~ChaseToRoam() override = default;

    bool IsTransition() override;

};

inline bool ChaseToRoam::IsTransition()
{
    return mAttribute->GetTickCount() > 10;
}
