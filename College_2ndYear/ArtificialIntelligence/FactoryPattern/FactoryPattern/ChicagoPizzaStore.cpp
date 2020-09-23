
#include "ChicagePizzaStore.h"

#include "ChicagoCheesePizza.h"
#include "ChicagoPeperoniPizza.h"


Pizza* ChicagoPizzaStore::CreatePizza(const EPizzaType pizzaType)
{
    Pizza* createdPizza = nullptr;

    switch (pizzaType)
    {
    case EPizzaType::Cheese:
        createdPizza = new ChicagoCheesePizza();
        break;
    case EPizzaType::Peperoni:
        createdPizza = new ChicagoPeperoniPizza();
        break;
    default:
        break;
    }

    mCreatedPizzas.emplace_back(createdPizza);

    return createdPizza;
}

