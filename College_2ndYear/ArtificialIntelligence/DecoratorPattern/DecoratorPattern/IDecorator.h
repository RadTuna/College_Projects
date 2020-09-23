#pragma once

#include "IComponent.h"

class IDecorator : public IComponent
{
public:
    IDecorator(IComponent* decorator) : mDecorator(decorator) {}
    virtual ~IDecorator() = default;

    std::string GetDescription() const override = 0
    {
        return "테스트 구문";
    }

protected:
    IComponent* mDecorator;

};
