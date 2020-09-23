#pragma once

#include "IComponent.h"

class Coffee final : public IComponent
{
public:
    Coffee(const std::string& description, float cost)
        : mCost(cost)
    {
        mDescription = description;
    }
    virtual ~Coffee() = default;

    float GetCost() override
    {
        return mCost;
    }
   
private:
    float mCost;

};
