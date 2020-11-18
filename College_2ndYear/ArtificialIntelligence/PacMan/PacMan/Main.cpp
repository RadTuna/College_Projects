
#include <cassert>

#include "PacMan.h"

int main(int argc, char** argv)
{
    enum { TICK_INTERVAL = 200 };

    if (argc != 2)
    {
        return -1;
    }

    PacMan::CreateInstance(TICK_INTERVAL);
    PacMan* pacMan = PacMan::GetInstance();
    assert(pacMan != nullptr);

    pacMan->LoadMap(argv[1]);
    pacMan->Run();

    PacMan::DeleteInstance();

    return 0;
}

