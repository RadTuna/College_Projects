
#include "Hunted.h"


void Hunted::EnterState()
{
    mAttribute->ResetTickCount();
    mAttribute->SetRenderColor(HIGH | BLUE);
}

void Hunted::UpdateState()
{
    Roam::UpdateState();
}

void Hunted::ExitState()
{
    mAttribute->SetRenderColor(HIGH | RED);
}
