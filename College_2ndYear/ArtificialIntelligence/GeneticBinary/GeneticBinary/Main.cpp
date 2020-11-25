
#include <iostream>
#include <random>

#include "Genetic.h"
#include "Node.h"


int main()
{
    enum { LOOP_LIMIT = 20, GENE_LENGTH = 6 };

    // Declare crossover function
    auto crossOverFunc = [](const std::vector<Node>& aGene, const std::vector<Node>& bGene) -> std::array<std::vector<Node>, 2>
    {
        assert(aGene.size() == bGene.size());

        std::array<std::vector<Node>, 2> outArray = {aGene, bGene};
        std::vector<Node>& copyAGene = outArray[0];
        std::vector<Node>& copyBGene = outArray[1];

        const size_t halfIndex = aGene.size() / 2;

        memcpy(copyAGene.data(), bGene.data(), sizeof(Node) * halfIndex);
        memcpy(copyBGene.data(), aGene.data(), sizeof(Node) * halfIndex);

        for (size_t i = halfIndex; i < aGene.size(); ++i)
        {
            const auto aEndIter = copyAGene.begin() + halfIndex;
            auto aTargetIter = std::find(copyAGene.begin(), aEndIter, copyAGene[i]);
            if (aTargetIter != aEndIter)
            {
                while (true)
                {
                    const size_t innerIndex = std::distance(copyAGene.begin(), aTargetIter);
                    auto innerTargetIter = std::find(copyAGene.begin(), aEndIter, copyBGene[innerIndex]);
                    if (innerTargetIter == aEndIter)
                    {
                        break;
                    }

                    aTargetIter = innerTargetIter;
                }

                const size_t index = std::distance(copyAGene.begin(), aTargetIter);

                const auto bStartIter = copyBGene.begin() + halfIndex;
                auto bTargetIter = std::find(bStartIter, copyBGene.end(), copyBGene[index]);
                if (bTargetIter != copyBGene.end())
                {
                    std::swap(copyAGene[i], *bTargetIter);
                }
            }
        }

        return outArray;
    };

    // Declare fitness function
    auto fitnessFunc = [](const std::vector<Node>& gene) -> Fitness
    {
        float fitness = 0.f;
        for (size_t i = 1; i < gene.size(); ++i)
        {
            const Node& startNode = gene[i - 1];
            const Node& endNode = gene[i];
            const float diffX = endNode.GetX() - startNode.GetX();
            const float diffY = endNode.GetY() - startNode.GetY();

            fitness += std::sqrtf(diffX * diffX + diffY * diffY);
        }
        return Fitness(fitness, EFitnessOrder::Less);
    };

    // Declare mutation function
    auto mutationFunc = [](std::vector<Node>& gene, float percentage) -> void
    {
        std::random_device randomDevice;
        std::mt19937 twister(randomDevice());
        std::uniform_real_distribution<float> dist(0.f, 1.f);
        std::uniform_int_distribution<size_t> distIndex(0, gene.size() - 1);

        for (size_t i = 0; i < gene.size() - 1; ++i)
        {
            if (percentage > dist(twister))
            {
                std::swap(gene[i], gene[distIndex(twister)]);
            }
        }
    };

    std::vector<Node> gene;
    gene.reserve(GENE_LENGTH);
    for (size_t i = 0; i < GENE_LENGTH; ++i)
    {
        gene.emplace_back(Node::GetRandomElement(Node(0.f, 0.f), Node(100.f, 100.f)));
    }

    Genetic<Node> genetic(
        10, 
        0.1f,
        gene,
        crossOverFunc,
        fitnessFunc,
        mutationFunc);

    std::cout << "AvgFitness : " << genetic.GetAverageFitness() << std::endl;
    for (size_t i = 0; i < LOOP_LIMIT; ++i)
    {
        genetic.Update();
        genetic.Draw();
        std::cout << "AvgFitness : " << genetic.GetAverageFitness() << std::endl;
    }
    std::cout << "Salesman traversal Complete!" << std::endl;

    return 0;
}
