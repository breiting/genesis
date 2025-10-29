#pragma once
#include <genesis/model/Agent.hpp>
#include <genesis/model/IFitnessEvaluator.hpp>
#include <memory>
#include <vector>

namespace gen {

/**
 * @brief Coordinates training: simulation, evaluation, and evolution.
 */
class Trainer {
   public:
    Trainer(std::vector<std::unique_ptr<Agent>>& agents, std::unique_ptr<IFitnessEvaluator> evaluator,
            const std::string& capabilityName);

    void RunGeneration(float dt, size_t steps, float mutationRate = 0.02f, float mutationMag = 0.05f,
                       float elitism = 0.2f);

    float GetAverageFitness() const {
        return m_AvgFitness;
    }
    float GetBestFitness() const {
        return m_BestFitness;
    }

    const Genome& GetBestGenome() const;

   private:
    std::vector<std::unique_ptr<Agent>>& m_Agents;
    std::unique_ptr<IFitnessEvaluator> m_Evaluator;
    std::string m_CapabilityName;

    float m_AvgFitness{0.0f};
    float m_BestFitness{0.0f};
};

}  // namespace gen
