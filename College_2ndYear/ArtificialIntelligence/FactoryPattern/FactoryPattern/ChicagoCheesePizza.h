#pragma once

#include "Pizza.h"

class ChicagoCheesePizza : public Pizza
{
public:
    ChicagoCheesePizza()
    {
        mDescription = "ChicagoCheesePizza";
    }
    virtual ~ChicagoCheesePizza() = default;

};
