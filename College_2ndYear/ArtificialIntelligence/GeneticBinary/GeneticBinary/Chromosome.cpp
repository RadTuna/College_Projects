
#include "Chromosome.h"

#include <cassert>

Chromosome::Chromosome(size_t geneLength, const Range& range, 
    const FitnessFunc& fitnessFunc, const MutationFunc& mutationFunc)
    : mGeneLength(geneLength)
    , mRange(range)
    , mFitnessFunc(fitnessFunc)
    , mMutationFunc(mutationFunc)
{
    mGene.reserve(geneLength);
    Reset();
}

Chromosome::Chromosome(const std::vector<int>& initGene, size_t geneLength, const Range& range,
    const FitnessFunc& fitnessFunc, const MutationFunc& mutationFunc)
    : mGeneLength(geneLength)
    , mRange(range)
    , mFitnessFunc(fitnessFunc)
    , mMutationFunc(mutationFunc)
{
    mGene.reserve(geneLength);
    Reset(initGene);
}

void Chromosome::Reset()
{
    std::random_device randomDevice;
    std::mt19937 twister(randomDevice());
    std::uniform_int_distribution<int> distribution(mRange.first, mRange.second);

    mGene.clear();

    for (size_t i = 0; i < mGeneLength; ++i)
    {
        mGene.emplace_back(distribution(twister));
    }
}

void Chromosome::Reset(const std::vector<int>& gene)
{
    mGene = gene;
}

int Chromosome::GetElement(size_t index) const
{
    assert(index < mGeneLength);

    return mGene[index];
}

const std::vector<int>& Chromosome::GetGene() const
{
    return mGene;
}

float Chromosome::GetFitness() const
{
    return mFitnessFunc(mGene);
}

void Chromosome::CrossOver(const Chromosome& other, std::array<std::unique_ptr<Chromosome>, 2>* outChild) const
{
    assert(outChild != nullptr);
    assert(mGeneLength == other.mGeneLength);

    std::random_device randomDevice;
    std::mt19937 twister(randomDevice());
    std::uniform_int_distribution<int> distribution(0, 1);

    const size_t halfLength = mGeneLength / 2;
    const size_t remain = mGeneLength % 2;

    std::vector<int> geneCopy = mGene;
    std::vector<int> otherGeneCopy = other.mGene;

    for (size_t i = 0; i < mGeneLength; ++i)
    {
        if (distribution(twister) == 1)
        {
            std::swap(geneCopy[i], otherGeneCopy[i]);
        }
    }

    (*outChild)[0] = std::make_unique<Chromosome>(geneCopy, mGeneLength, mRange, mFitnessFunc, mMutationFunc);
    (*outChild)[1] = std::make_unique<Chromosome>(otherGeneCopy, mGeneLength, mRange, mFitnessFunc, mMutationFunc);
}

void Chromosome::Mutation(float percentage)
{
    assert(percentage >= 0.f && percentage <= 1.f);

    std::random_device randomDevice;
    std::mt19937 twister(randomDevice());
    std::uniform_real_distribution<float> distribution(0.f, 1.f);

    for (size_t i = 0; i < mGeneLength; ++i)
    {
        if (percentage > distribution(twister))
        {
            mGene[i] = mMutationFunc(mGene[i]);
        }
    }
}
