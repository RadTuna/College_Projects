#pragma once

#include <string>

class IComponent
{
public:
    IComponent() = default;
    virtual ~IComponent() = default;

    virtual std::string GetDescription() const
    {
        return mDescription;
    }
    virtual float GetCost() = 0;

protected:
    std::string mDescription;

};
