#pragma once

#include "Pizza.h"

class ChicagoPeperoniPizza : public Pizza
{
public:
    ChicagoPeperoniPizza()
    {
        mDescription = "ChicagoPeperoniPizza";
    }
    virtual ~ChicagoPeperoniPizza() = default;

};
