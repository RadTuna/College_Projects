#pragma once

enum class EMoveDirection
{
    Up,
    Down,
    Left,
    Right,
    Stop
};

enum class EObjectLayer
{
    Wall,
    Pill,
    PowerPill,
    Ghost,
    Player
};

#define RED (1 << 2)
#define GREEN (1 << 1)
#define BLUE (1)

#define WHITE (RED | GREEN | BLUE)

#define HIGH (1 << 3)
