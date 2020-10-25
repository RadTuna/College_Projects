#pragma once

#include "Transition.h"
#include "Ghost.h"

class RoamToChase final : public Transition<Ghost>
{
public:
    explicit RoamToChase() = default;
    ~RoamToChase() override = default;

    bool IsTransition() override;

};

inline bool RoamToChase::IsTransition()
{
    return mAttribute->GetTickCount() > 20;
}
