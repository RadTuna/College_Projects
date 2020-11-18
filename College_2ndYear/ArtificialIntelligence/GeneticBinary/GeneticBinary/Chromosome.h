#pragma once

#include <vector>
#include <functional>
#include <array>
#include <memory>
#include <random>
#include <utility>

typedef std::function<float(const std::vector<int>&)> FitnessFunc;
typedef std::function<int(int)> MutationFunc;
typedef std::pair<int, int> Range;

class Chromosome final
{
public:
    Chromosome(size_t geneLength, const Range& range, 
        const FitnessFunc& fitnessFunc, const MutationFunc& mutationFunc);
    Chromosome(const std::vector<int>& initGene, size_t geneLength, const Range& range, 
        const FitnessFunc& fitnessFunc, const MutationFunc& mutationFunc);
    ~Chromosome() = default;

    void Reset();
    void Reset(const std::vector<int>& gene);
    int GetElement(size_t index) const;
    const std::vector<int>& GetGene() const;
    float GetFitness() const;

    void CrossOver(const Chromosome& other, std::array<std::unique_ptr<Chromosome>, 2>* outChild) const;
    void Mutation(float percentage);

private:
    std::vector<int> mGene;
    size_t mGeneLength;
    Range mRange;

    FitnessFunc mFitnessFunc;
    MutationFunc mMutationFunc;

};
