
#include "Genetic.h"

#include <limits>
#include <iostream>

Genetic::Genetic(size_t chromosomeLength, float mutationPercentage, 
    size_t geneLength, const Range& range, 
    const FitnessFunc& fitnessFunc, const MutationFunc& mutationFunc)
    : mChromosomeLength(chromosomeLength)
    , mMutationPercentage(mutationPercentage)
{
    mChromosomes.reserve(mChromosomeLength);

    for (size_t i = 0; i < mChromosomeLength; ++i)
    {
        mChromosomes.emplace_back(std::make_unique<Chromosome>(geneLength, range, fitnessFunc, mutationFunc));
    }

    Reset();
}

void Genetic::Reset()
{
    for (std::unique_ptr<Chromosome>& chromosome : mChromosomes)
    {
        chromosome->Reset();
    }
}

void Genetic::Update()
{
    size_t halfLength = mChromosomeLength / 2;
    std::vector<std::pair<float, size_t>> bestChromosomes;
    std::vector<size_t> worstChromosome;

    if (mChromosomeLength % 2 == 1)
    {
        ++halfLength;
    }

    // Selection stage
    for (size_t i = 0; i < mChromosomeLength; ++i)
    {
        const float fitness = mChromosomes[i]->GetFitness();
        if (bestChromosomes.size() < halfLength)
        {
            bestChromosomes.emplace_back(std::make_pair(fitness, i));
        }
        else
        {
            size_t minIndex = 0;
            for (size_t j = 0; j < bestChromosomes.size(); ++j)
            {
                if (bestChromosomes[minIndex].first > bestChromosomes[j].first)
                {
                    minIndex = j;
                }
            }

            if (bestChromosomes[minIndex].first < fitness)
            {
                worstChromosome.emplace_back(bestChromosomes[minIndex].second);
                bestChromosomes[minIndex] = std::make_pair(fitness, i);
            }
            else
            {
                worstChromosome.emplace_back(i);
            }
        }
    }

    // CrossOver stage
    for (size_t i = 0; i < worstChromosome.size(); i += 2)
    {
        const size_t invIndex = bestChromosomes.size() - (i + 1);

        std::array<std::unique_ptr<Chromosome>, 2> childChromosome;
        size_t firstIndex = bestChromosomes[i].second;
        size_t secondIndex = bestChromosomes[invIndex].second;

        mChromosomes[firstIndex]->CrossOver(*mChromosomes[secondIndex], &childChromosome);

        firstIndex = worstChromosome[i];
        secondIndex = worstChromosome[(i + 1) % worstChromosome.size()];

        mChromosomes[firstIndex] = std::move(childChromosome[0]);
        mChromosomes[secondIndex] = std::move(childChromosome[1]);
    }

    // Mutation stage
    for (std::unique_ptr<Chromosome>& chromosome : mChromosomes)
    {
        chromosome->Mutation(mMutationPercentage);
    }
}

void Genetic::Draw()
{
    for (size_t i = 0; i < mChromosomeLength; ++i)
    {
        std::cout << "[" << i << "] ";
        for (const int& gene : mChromosomes[i]->GetGene())
        {
            std::cout << gene << " ";
        }
        std::cout << " => " << mChromosomes[i]->GetFitness() << std::endl;
    }
}

float Genetic::GetAverageFitness() const
{
    float totalFitness = 0.f;
    for (const std::unique_ptr<Chromosome>& chromosome : mChromosomes)
    {
        totalFitness += chromosome->GetFitness();
    }

    return totalFitness / mChromosomeLength;
}
