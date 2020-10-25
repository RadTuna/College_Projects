#pragma once


namespace Key
{
    enum Enum
    {
        Up = 0x26,
        Down = 0x28,
        Left = 0x25,
        Right = 0x27,
        End = 0x23
    };
}

namespace InputManager
{

    bool IsPressed(Key::Enum key);

}
