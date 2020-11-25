#pragma once

#include <vector>
#include <functional>
#include <array>
#include <memory>
#include <random>
#include <utility>



enum class EFitnessOrder
{
    Greater,
    Less
};

struct Fitness
{
    Fitness() = default;
    Fitness(float value, EFitnessOrder order) : Value(value), Order(order) {}

    float Value;
    EFitnessOrder Order;
};

template<typename Type>
using CrossOverFunc = std::function<std::array<std::vector<Type>, 2>(const std::vector<Type>&, const std::vector<Type>&)>;

template<typename Type>
using FitnessFunc = std::function<Fitness(const std::vector<Type>&)>;

template<typename Type>
using MutationFunc = std::function<void(std::vector<Type>&, float)>;

template<typename Type>
using Range = std::pair<Type, Type>;

template<typename Type>
class Chromosome final
{
public:
    Chromosome(size_t geneLength, const Range<Type>& range,
        const CrossOverFunc<Type>& crossOverFunc,
        const FitnessFunc<Type>& fitnessFunc, 
        const MutationFunc<Type>& mutationFunc);
    Chromosome(const std::vector<Type>& initGene, size_t geneLength,
        const Range<Type>& range,
        const CrossOverFunc<Type>& crossOverFunc,
        const FitnessFunc<Type>& fitnessFunc, 
        const MutationFunc<Type>& mutationFunc);
    ~Chromosome() = default;

    void Reset();
    void Reset(const std::vector<Type>& gene);
    int GetElement(size_t index) const;
    const std::vector<Type>& GetGene() const;
    Fitness GetFitness() const;

    void CrossOver(const Chromosome& other, std::array<std::unique_ptr<Chromosome>, 2>* outChild) const;
    void Mutation(float percentage);

private:
    std::vector<Type> mGene;
    size_t mGeneLength;
    Range<Type> mRange;

    CrossOverFunc<Type> mCrossOverFunc;
    FitnessFunc<Type> mFitnessFunc;
    MutationFunc<Type> mMutationFunc;

};

#include "Chromosome.hpp"
