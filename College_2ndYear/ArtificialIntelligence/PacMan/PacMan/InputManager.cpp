
#include "InputManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace InputManager
{

    bool InputManager::IsPressed(Key::Enum key)
    {
        return GetAsyncKeyState(key);
    }

}
