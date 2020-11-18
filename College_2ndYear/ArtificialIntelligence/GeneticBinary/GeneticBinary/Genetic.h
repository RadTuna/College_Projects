#pragma once

#include <vector>
#include <memory>
#include "Chromosome.h"

class Genetic final
{
public:
    Genetic(size_t chromosomeLength, float mutationPercentage, 
        size_t geneLength, const Range& range, 
        const FitnessFunc& fitnessFunc, const MutationFunc& mutationFunc);

    void Reset();
    void Update();
    void Draw();
    float GetAverageFitness() const;

private:
    std::vector<std::unique_ptr<Chromosome>> mChromosomes;
    size_t mChromosomeLength;

    float mMutationPercentage;
};