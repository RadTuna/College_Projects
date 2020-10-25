
#include "Dead.h"


void Dead::EnterState()
{
    mAttribute->SetDead(true);
    mAttribute->SetVisible(false);
    mAttribute->SetLocation(mAttribute->GetStartLocation());
}

void Dead::UpdateState()
{
}

void Dead::ExitState()
{
    mAttribute->SetDead(false);
    mAttribute->SetVisible(true);
}
