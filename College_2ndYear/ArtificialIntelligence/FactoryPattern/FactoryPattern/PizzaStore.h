#pragma once

#include <vector>
#include "Pizza.h"
#include "PizzaType.h"

class PizzaStore
{
public:
    PizzaStore();
    virtual ~PizzaStore();

    virtual Pizza* CreatePizza(const EPizzaType pizzaType) = 0;
    void DestroyPizza();

protected:
    std::vector<Pizza*> mCreatedPizzas;

};
