#pragma once

#include "Transition.h"
#include "Ghost.h"

class DeadToRoam final : public Transition<Ghost>
{
public:
    explicit DeadToRoam() = default;
    ~DeadToRoam() override = default;

    bool IsTransition() override;

};

inline bool DeadToRoam::IsTransition()
{
    return mAttribute->GetTickCount() > 30;
}
