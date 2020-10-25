
#include <cassert>

#include "PacMan.h"

int main()
{
    enum { TICK_INTERVAL = 200 };

    PacMan::CreateInstance(TICK_INTERVAL);
    PacMan* pacMan = PacMan::GetInstance();
    assert(pacMan != nullptr);

    pacMan->LoadMap("../Resource/Map.txt");
    pacMan->Run();

    PacMan::DeleteInstance();

    return 0;
}

