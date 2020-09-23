#pragma once

#include "PizzaStore.h"

class ChicagoPizzaStore : public PizzaStore
{
public:
    ChicagoPizzaStore() = default;
    ~ChicagoPizzaStore() override = default;

    Pizza* CreatePizza(const EPizzaType pizzaType) override;

};
