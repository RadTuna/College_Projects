#pragma once

#include "IDecorator.h"

class Mocha final : public IDecorator
{
public:
    Mocha(IComponent* decorator, std::string description, float cost)
        : IDecorator(decorator)
    {
        mDescription = description;
        mCost = cost;
    }
    virtual ~Mocha() = default;

    std::string GetDescription() const override
    {
        std::string childDescription = IDecorator::GetDescription();
        if (mDecorator != nullptr)
        {
            childDescription += mDecorator->GetDescription();
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
