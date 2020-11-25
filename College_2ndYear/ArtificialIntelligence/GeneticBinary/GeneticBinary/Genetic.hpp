#pragma once

#include "Genetic.h"

#include <limits>
#include <iostream>

template<typename Type>
Genetic<Type>::Genetic(size_t chromosomeLength, float mutationPercentage, size_t geneLength,
    const Range<Type>& range,
    const CrossOverFunc<Type>& crossOverFunc,
    const FitnessFunc<Type>& fitnessFunc, 
    const MutationFunc<Type>& mutationFunc)
    : mChromosomeLength(chromosomeLength)
    , mMutationPercentage(mutationPercentage)
{
    mChromosomes.reserve(mChromosomeLength);

    for (size_t i = 0; i < mChromosomeLength; ++i)
    {
        mChromosomes.emplace_back(std::make_unique<Chromosome<Type>>(
            geneLength, 
            range, 
            crossOverFunc, 
            fitnessFunc, 
            mutationFunc));
    }
}

template <typename Type>
Genetic<Type>::Genetic(size_t chromosomeLength, float mutationPercentage,
    std::vector<Type> initGene,
    const CrossOverFunc<Type>& crossOverFunc, 
    const FitnessFunc<Type>& fitnessFunc,
    const MutationFunc<Type>& mutationFunc)
    : mChromosomeLength(chromosomeLength)
    , mMutationPercentage(mutationPercentage)
{
    mChromosomes.reserve(mChromosomeLength);

    std::random_device randomDevice;
    std::mt19937 twister(randomDevice());

    for (size_t i = 0; i < mChromosomeLength; ++i)
    {
        std::shuffle(initGene.begin(), initGene.end(), twister);

        mChromosomes.emplace_back(std::make_unique<Chromosome<Type>>(
            initGene, 
            initGene.size(), 
            std::make_pair(Type(), Type()), 
            crossOverFunc, 
            fitnessFunc, 
            mutationFunc));
    }
}

template<typename Type>
void Genetic<Type>::Reset()
{
    for (std::unique_ptr<Chromosome<Type>>& chromosome : mChromosomes)
    {
        chromosome->Reset();
    }
}

template<typename Type>
void Genetic<Type>::Update()
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
        const Fitness fitness = mChromosomes[i]->GetFitness();
        if (bestChromosomes.size() < halfLength)
        {
            bestChromosomes.emplace_back(std::make_pair(fitness.Value, i));
        }
        else
        {
            const bool bIsGreaterOrder = fitness.Order == EFitnessOrder::Greater;

            size_t minIndex = 0;
            for (size_t j = 0; j < bestChromosomes.size(); ++j)
            {
                const bool bIsLessValue = bestChromosomes[minIndex].first > bestChromosomes[j].first;
                if (bIsLessValue == bIsGreaterOrder)
                {
                    minIndex = j;
                }
            }

            const bool bIsGreaterValue = bestChromosomes[minIndex].first < fitness.Value;
            if (bIsGreaterValue == bIsGreaterOrder)
            {
                worstChromosome.emplace_back(bestChromosomes[minIndex].second);
                bestChromosomes[minIndex] = std::make_pair(fitness.Value, i);
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

        std::array<std::unique_ptr<Chromosome<Type>>, 2> childChromosome;
        size_t firstIndex = bestChromosomes[i].second;
        size_t secondIndex = bestChromosomes[invIndex].second;

        mChromosomes[firstIndex]->CrossOver(*mChromosomes[secondIndex], &childChromosome);

        firstIndex = worstChromosome[i];
        secondIndex = worstChromosome[(i + 1) % worstChromosome.size()];

        mChromosomes[firstIndex] = std::move(childChromosome[0]);
        mChromosomes[secondIndex] = std::move(childChromosome[1]);
    }

    // Mutation stage
    for (std::unique_ptr<Chromosome<Type>>& chromosome : mChromosomes)
    {
        chromosome->Mutation(mMutationPercentage);
    }
}

template<typename Type>
void Genetic<Type>::Draw()
{
    for (size_t i = 0; i < mChromosomeLength; ++i)
    {
        std::cout << "[" << i << "] ";
        for (const Type& gene : mChromosomes[i]->GetGene())
        {
            std::cout << gene << " ";
        }
        std::cout << " => " << mChromosomes[i]->GetFitness().Value << std::endl;
    }
}

template<typename Type>
float Genetic<Type>::GetAverageFitness() const
{
    float totalFitness = 0.f;
    for (const std::unique_ptr<Chromosome<Type>>& chromosome : mChromosomes)
    {
        totalFitness += chromosome->GetFitness().Value;
    }

    return totalFitness / mChromosomeLength;
}
