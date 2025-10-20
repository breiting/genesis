#include <genesis/core/Math.hpp>
#include <genesis/model/evolution/Genome.hpp>
#include <genesis/model/evolution/NeuralNetwork.hpp>
#include <genesis/model/evolution/Population.hpp>
#include <genesis/model/evolution/XorEvaluator.hpp>
#include <iostream>
#include <memory>

using namespace gen;
using namespace std;

constexpr size_t populationSize = 50;
constexpr int generations = 100;

void test(const Genome& best) {
    std::vector<int> layout = {2, 4, 1};
    auto nn = std::make_shared<NeuralNetwork>(layout);
    nn->SetWeightsAndBiases(best.GetDNA());

    nn->ExportDot("nn.dot");

    std::cout << "\nTesting best genome:\n";

    std::vector<std::vector<float>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    std::vector<float> targets = {0, 1, 1, 0};

    int correct = 0;
    for (size_t i = 0; i < inputs.size(); ++i) {
        auto out = nn->Forward(inputs[i]);
        int predicted = (out[0] > 0.5f) ? 1 : 0;
        if (predicted == static_cast<int>(targets[i])) correct++;
        std::cout << inputs[i][0] << " XOR " << inputs[i][1] << " => " << predicted << " (target=" << targets[i]
                  << ")\n";
    }
    std::cout << "Accuracy: " << (100.0f * correct / inputs.size()) << "%\n";
}

int main() {
    std::vector<int> layout = {2, 4, 1};
    auto nn = std::make_shared<NeuralNetwork>(layout);
    size_t geneCount = nn->GetWeightsAndBiases().size();

    // Create population
    Population population(populationSize, geneCount);

    // Create XOR fitness evaluator
    XorEvaluator evaluator(nn);

    // Evolution loop
    for (int generation = 0; generation < generations; ++generation) {
        float bestFitness = 0.0f;

        // Evaluate all genomes
        for (auto& genome : population.GetGenomes()) {
            float fitness = evaluator.Evaluate(genome);
            genome.SetFitness(fitness);
            if (fitness > bestFitness) bestFitness = fitness;
        }

        // Evolve to next generation
        population.Evolve(0.1f, 0.3f, 0.1f);

        std::cout << "Generation " << generation << " | Best Fitness: " << bestFitness << std::endl;

        if (bestFitness > 0.99f) {
            std::cout << "✅ XOR solved at generation " << generation << "!\n";
            break;
        }
    }

    // Save the best genome
    const Genome& best = population.GetBest();
    best.SaveBinary("best_xor_genome.bin");
    std::cout << "Best genome saved.\n";

    test(best);

    return 0;
}
