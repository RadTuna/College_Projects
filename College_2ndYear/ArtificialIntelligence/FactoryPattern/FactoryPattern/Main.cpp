

#include <memory>

#include "ChicagePizzaStore.h"


int main()
{
    std::unique_ptr<PizzaStore> pizzaStore = std::make_unique<ChicagoPizzaStore>();

    Pizza* chicagoCheesePizza = pizzaStore->CreatePizza(EPizzaType::Cheese);
    Pizza* chicagoPeperoniPizza = pizzaStore->CreatePizza(EPizzaType::Peperoni);

    chicagoCheesePizza->PrintDescription();
    chicagoPeperoniPizza->PrintDescription();

    return 0;
}