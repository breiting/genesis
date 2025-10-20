#pragma once
#include <memory>

#include "Genome.hpp"
#include "NeuralNetwork.hpp"

namespace gen {

/**
 * @brief Simple evaluator that measures how well a neural network learns XOR logic.
 */
class XorEvaluator {
   public:
    explicit XorEvaluator(std::shared_ptr<NeuralNetwork> nn) : m_Network(nn) {
    }

    float Evaluate(const Genome& genome) {
        m_Network->SetWeightsAndBiases(genome.GetDNA());

        const std::vector<std::vector<float>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
        const std::vector<float> targets = {0, 1, 1, 0};

        float totalError = 0.0f;
        for (size_t i = 0; i < inputs.size(); ++i) {
            auto out = m_Network->Forward(inputs[i]);
            float diff = std::fabs(out[0] - targets[i]);
            totalError += diff;
        }

        // fitness = 1 - normalized error
        float fitness = 1.0f - (totalError / static_cast<float>(inputs.size()));
        return fitness;
    }

   private:
    std::shared_ptr<NeuralNetwork> m_Network;
};

}  // namespace gen
