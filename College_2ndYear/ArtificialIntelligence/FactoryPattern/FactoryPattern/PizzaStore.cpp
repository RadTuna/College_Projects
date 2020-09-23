
#include "PizzaStore.h"

PizzaStore::PizzaStore()
{
    mCreatedPizzas.reserve(32);
}

PizzaStore::~PizzaStore()
{
    DestroyPizza();
}

void PizzaStore::DestroyPizza()
{
        for (Pizza* pizza : mCreatedPizzas)
    {
        if (pizza != nullptr)
        {
            delete pizza;
        }
    }
}
