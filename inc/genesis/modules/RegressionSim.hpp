#pragma once

#include <genesis/model/RegressionEvaluator.hpp>
#include <genesis/model/Trainer.hpp>
#include <genesis/modules/ISimModule.hpp>
#include <memory>
#include <vector>

namespace gen {

/**
 * @brief Simulation module for functional regression using neuroevolution.
 *
 * This module evolves neural networks to perform regression tasks, such as
 * predicting stock prices from historical data. It manages a population of
 * agents, each with a "regression" capability, and uses a RegressionEvaluator
 * to compute fitness based on prediction accuracy.
 */
class RegressionSim : public ISimModule {
   public:
    RegressionSim();

    std::string Name() const override;
    void OnAttach() override;
    void OnDetach() override;
    void Update(double dt) override;
    void RenderWorld(const glm::mat4& vp) override;
    void RenderImGui() override;

   private:
    /**
     * @brief Loads regression data from a CSV file.
     * @param filename Path to CSV file (expects one column of float values).
     * @param windowSize Number of past values to use as inputs.
     */
    void LoadData(const std::string& filename, size_t windowSize);

    /**
     * @brief Resets the simulation: loads data, rebuilds population, resets trainer.
     */
    void Reset() override;

    /**
     * @brief Rebuilds the population of agents with regression capabilities.
     */
    void RebuildPopulation();

    /**
     * @brief Runs one generation of evolution.
     */
    void TrainOneGeneration();

    std::vector<std::vector<float>> m_Inputs;          ///< Input data for regression.
    std::vector<float> m_Targets;                      ///< Target outputs.
    std::vector<std::unique_ptr<Agent>> m_Agents;      ///< Population of agents.
    std::unique_ptr<Trainer> m_Trainer;                ///< Handles evolution.
    std::unique_ptr<RegressionEvaluator> m_Evaluator;  ///< Evaluates fitness.

    // Parameters
    int m_NumAgents = 100;                   ///< Population size.
    float m_MutationRate = 0.05f;            ///< Mutation probability.
    float m_MutationMag = 0.5f;              ///< Mutation magnitude.
    float m_Elitism = 0.1f;                  ///< Elite fraction.
    size_t m_WindowSize = 5;                 ///< Input window size.
    std::string m_DataPath = "../data.csv";  ///< Path to data file.
    int m_GenerationsPerClick = 1;           ///< Generations per update.
    bool m_IsTraining = false;               ///< Whether to train or observe.
};

}  // namespace gen
