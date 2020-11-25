#pragma once

#include <vector>
#include <memory>
#include "Chromosome.h"

template<typename Type>
class Genetic final
{
public:
    Genetic(size_t chromosomeLength, float mutationPercentage, size_t geneLength,
        const Range<Type>& range,
        const CrossOverFunc<Type>& crossOverFunc,
        const FitnessFunc<Type>& fitnessFunc, 
        const MutationFunc<Type>& mutationFunc);

    Genetic(size_t chromosomeLength, float mutationPercentage,
        std::vector<Type> initGene,
        const CrossOverFunc<Type>& crossOverFunc,
        const FitnessFunc<Type>& fitnessFunc, 
        const MutationFunc<Type>& mutationFunc);

    void Reset();
    void Update();
    void Draw();
    float GetAverageFitness() const;

private:
    std::vector<std::unique_ptr<Chromosome<Type>>> mChromosomes;
    size_t mChromosomeLength;

    float mMutationPercentage;
};

#include "Genetic.hpp"
