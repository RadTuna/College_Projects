#pragma once

class IDisplay
{
public:
    explicit IDisplay() = default;
    virtual ~IDisplay() = default;

    virtual void Display() = 0;
};
