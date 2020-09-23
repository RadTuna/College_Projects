
#include <memory>
#include <iostream>
#include "Coffee.h"
#include "Mocha.h"
#include "Whip.h"

int main()
{
    std::unique_ptr<Coffee> coffee = std::make_unique<Coffee>("Ŀ��", 2.f);
    std::unique_ptr<Mocha> mocha = std::make_unique<Mocha>(coffee.get(), "��ī", 1.5f);
    std::unique_ptr<Whip> whip = std::make_unique<Whip>(mocha.get(), "����", 1.f);

    std::cout << "���� : " << whip->GetDescription() << std::endl;
    std::cout << "���� : " << whip->GetCost() << std::endl;

    return 0;
}
