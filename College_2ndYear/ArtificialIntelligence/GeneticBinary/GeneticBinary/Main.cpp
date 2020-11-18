
#include <iostream>

#include "Genetic.h"

int main()
{
    enum { LOOP_LIMIT = 40 };

    auto fitnessFunc = [](const std::vector<int>& gene) -> float
    {
        float fitness = 0.f;
        for (int element : gene)
        {
            fitness += element == 1 ? 1.f : 0.f;
        }
        return fitness;
    };

    auto mutationFunc = [](int element) -> int
    {
        return element == 1 ? 0 : 1;
    };

    Genetic genetic(
        20, 
        0.01f, 
        50, 
        std::make_pair(0, 1), 
        fitnessFunc,
        mutationFunc);

    std::cout << "AvgFitness : " << genetic.GetAverageFitness() << std::endl;
    for (size_t i = 0; i < LOOP_LIMIT; ++i)
    {
        genetic.Update();
        genetic.Draw();
        std::cout << "AvgFitness : " << genetic.GetAverageFitness() << std::endl;
    }
    std::cout << "OneMax Complete!" << std::endl;

    return 0;
}
