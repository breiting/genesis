#pragma once

#include <genesis/model/IFitnessEvaluator.hpp>
#include <vector>

namespace gen {

/**
 * @brief Evaluates fitness for functional regression tasks.
 *
 * This evaluator feeds input data to an agent's neural network and computes
 * fitness based on how well the network's outputs match target values.
 * Uses Mean Squared Error (MSE) as the error metric, with fitness = 1 / (1 + MSE)
 * to convert minimization to maximization.
 */
class RegressionEvaluator : public IFitnessEvaluator {
   public:
    /**
     * @brief Constructs a RegressionEvaluator with input-target pairs.
     * @param inputs Vector of input vectors (e.g., historical prices).
     * @param targets Vector of target values (e.g., next prices).
     */
    RegressionEvaluator(const std::vector<std::vector<float>>& inputs, const std::vector<float>& targets);

    /**
     * @brief Evaluates an agent's fitness by running regression on the dataset.
     * @param agent The agent whose neural network is evaluated.
     * @return Fitness score (higher is better).
     */
    float Evaluate(Agent& agent) override;

   private:
    std::vector<std::vector<float>> m_Inputs;  ///< Input data for regression.
    std::vector<float> m_Targets;              ///< Target outputs.
};

}  // namespace gen