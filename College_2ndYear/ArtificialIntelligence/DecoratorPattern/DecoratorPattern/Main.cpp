
#include <memory>
#include <iostream>
#include "Coffee.h"
#include "Mocha.h"
#include "Whip.h"

int main()
{
    std::unique_ptr<Coffee> coffee = std::make_unique<Coffee>("커피", 2.f);
    std::unique_ptr<Mocha> mocha = std::make_unique<Mocha>(coffee.get(), "모카", 1.5f);
    std::unique_ptr<Whip> whip = std::make_unique<Whip>(mocha.get(), "휘핑", 1.f);

    std::cout << "종류 : " << whip->GetDescription() << std::endl;
    std::cout << "가격 : " << whip->GetCost() << std::endl;

    return 0;
}
