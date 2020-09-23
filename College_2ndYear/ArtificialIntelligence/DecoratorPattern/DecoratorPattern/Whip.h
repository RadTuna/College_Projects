#pragma once

#include "IDecorator.h"

class Whip final : public IDecorator
{
public:
    Whip(IComponent* decorator, std::string description, float cost)
        : IDecorator(decorator)
    {
        mDescription = description;
        mCost = cost;
    }
    virtual ~Whip() = default;

    std::string GetDescription() const override
    {
        std::string childDescription;
        if (mDecorator != nullptr)
        {
            childDescription = mDecorator->GetDescription();
        }
        return mDescription + childDescription;
    }

    float GetCost() override
    {
        float childCost = 0.f;
        if (mDecorator != nullptr)
        {
            childCost = mDecorator->GetCost();
        }

        return mCost + childCost;
    }

private:
    float mCost;

};
