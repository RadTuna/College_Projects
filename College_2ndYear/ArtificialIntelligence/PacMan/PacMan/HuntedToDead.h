#pragma once

#include "Transition.h"
#include "Ghost.h"

class HuntedToDead final : public Transition<Ghost>
{
public:
    explicit HuntedToDead() = default;
    ~HuntedToDead() override = default;

    bool IsTransition() override;

};

inline bool HuntedToDead::IsTransition()
{
    return mAttribute->IsDead();
}
