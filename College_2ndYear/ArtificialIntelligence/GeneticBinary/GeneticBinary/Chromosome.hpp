#pragma once

#include "Chromosome.h"

#include <cassert>

template<typename Type>
Chromosome<Type>::Chromosome(size_t geneLength, const Range<Type>& range,
    const CrossOverFunc<Type>& crossOverFunc,
    const FitnessFunc<Type>& fitnessFunc, 
    const MutationFunc<Type>& mutationFunc)
    : mGeneLength(geneLength)
    , mRange(range)
    , mCrossOverFunc(crossOverFunc)
    , mFitnessFunc(fitnessFunc)
    , mMutationFunc(mutationFunc)
{
    mGene.reserve(geneLength);
    Reset();
}

template<typename Type>
Chromosome<Type>::Chromosome(const std::vector<Type>& initGene, size_t geneLength,
    const Range<Type>& range,
    const CrossOverFunc<Type>& crossOverFunc,
    const FitnessFunc<Type>& fitnessFunc, 
    const MutationFunc<Type>& mutationFunc)
    : mGeneLength(geneLength)
    , mRange(range)
    , mCrossOverFunc(crossOverFunc)
    , mFitnessFunc(fitnessFunc)
    , mMutationFunc(mutationFunc)
{
    mGene.reserve(geneLength);
    Reset(initGene);
}

template<typename Type>
void Chromosome<Type>::Reset()
{
    mGene.clear();

    for (size_t i = 0; i < mGeneLength; ++i)
    {
        mGene.emplace_back(Type::GetRandomElement(mRange.first, mRange.second));
    }
}

template<typename Type>
void Chromosome<Type>::Reset(const std::vector<Type>& gene)
{
    mGene = gene;
}

template<typename Type>
int Chromosome<Type>::GetElement(size_t index) const
{
    assert(index < mGeneLength);

    return mGene[index];
}

template<typename Type>
const std::vector<Type>& Chromosome<Type>::GetGene() const
{
    return mGene;
}

template<typename Type>
Fitness Chromosome<Type>::GetFitness() const
{
    return mFitnessFunc(mGene);
}

template<typename Type>
void Chromosome<Type>::CrossOver(const Chromosome<Type>& other, std::array<std::unique_ptr<Chromosome>, 2>* outChild) const
{
    assert(outChild != nullptr);
    assert(mGeneLength == other.mGeneLength);

    std::array<std::vector<Type>, 2> childGene = mCrossOverFunc(mGene, other.mGene);

    (*outChild)[0] = std::make_unique<Chromosome<Type>>(childGene[0], childGene[0].size(), mRange, mCrossOverFunc, mFitnessFunc, mMutationFunc);
    (*outChild)[1] = std::make_unique<Chromosome<Type>>(childGene[1], childGene[1].size(), mRange, mCrossOverFunc, mFitnessFunc, mMutationFunc);
}

template<typename Type>
void Chromosome<Type>::Mutation(float percentage)
{
    assert(percentage >= 0.f && percentage <= 1.f);

    mMutationFunc(mGene, percentage);
}
