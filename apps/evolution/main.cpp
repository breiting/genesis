#include <genesis/core/Math.hpp>
#include <genesis/model/agent/MovementBehavior.hpp>
#include <genesis/model/environment/PathEnvironment.hpp>
#include <genesis/model/evolution/Genome.hpp>
#include <genesis/model/evolution/NeuralNetwork.hpp>
#include <genesis/model/evolution/PathEvaluator.hpp>
#include <genesis/model/evolution/Population.hpp>
#include <genesis/model/evolution/XorEvaluator.hpp>
#include <iostream>

using namespace gen;
using namespace std;

int main() {
    auto nn = std::make_shared<NeuralNetwork>(std::vector<int>{4, 8, 2});
    size_t weightCount = nn->GetWeightsAndBiases().size();

    // Nur 1 Agent / 1 Genome
    Population pop(1, weightCount);
    PathEnvironment env(1);
    PathEvaluator eval(nn, env);

    const size_t generations = 5;

    for (size_t gen = 0; gen < generations; ++gen) {
        auto& genome = pop.GetGenomes()[0];
        float fitness = eval.Evaluate(genome);
        genome.SetFitness(fitness);

        std::cout << "Generation " << gen << " | Fitness: " << fitness << std::endl;

        // nur minimale Mutation
        genome.Mutate(0.5f, 0.1f);
    }

    const auto& best = pop.GetBest();
    std::cout << "Best fitness: " << best.GetFitness() << std::endl;
    best.SaveBinary("best_single_agent.genome");

    return 0;
}
