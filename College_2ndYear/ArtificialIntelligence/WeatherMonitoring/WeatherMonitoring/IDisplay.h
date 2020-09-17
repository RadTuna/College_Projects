#pragma once

class IDisplay
{
public:
    IDisplay() = default;
    ~IDisplay() = default;

    virtual void Display() = 0;
};
