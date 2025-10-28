#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "genesis/core/Math.hpp"
#include "genesis/model/Agent.hpp"
#include "genesis/model/MovementEvaluator.hpp"
#include "genesis/model/Trainer.hpp"
#include "genesis/modules/ISimModule.hpp"
#include "genesis/view/AgentView.hpp"
#include "genesis/view/CanvasView.hpp"
#include "genesis/view/TargetView.hpp"

namespace gen {

/**
 * @brief Neuro-evolution simulation module.
 *
 * Handles population training, evaluation, and visual observation.
 * Encapsulates all logic that was previously inside EvolutionApp.
 */
class NeuroEvolutionSim : public ISimModule {
   public:
    NeuroEvolutionSim();
    ~NeuroEvolutionSim() override = default;

    std::string Name() const override {
        return "Neuro-Evolution";
    }

    void OnAttach() override;
    void Reset() override;
    void Update(double dt) override;
    void RenderWorld(const glm::mat4& vp) override;
    void RenderImGui() override;

   private:
    void CreateAgent(const glm::vec2& pos, const glm::vec2& target);
    void RepositionAgents(bool random);
    void TrainGeneration();

   private:
    std::vector<std::unique_ptr<Agent>> m_Agents;
    std::unique_ptr<Trainer> m_Trainer;
    std::unique_ptr<MovementEvaluator> m_Evaluator;

    AgentView m_AgentView;
    TargetView m_StartView;
    TargetView m_TargetView;
    CanvasView m_CanvasView;

    glm::vec2 m_StartPos{-30.0f, 0.0f};
    glm::vec2 m_TargetPos{0.0f, 0.0f};
    glm::vec2 m_WorldSize{80.0f, 80.0f};

    int m_NumAgents = 100;
    int m_GenerationCount = 0;
    bool m_IsObserving = false;

    float m_Timescale = 1.0f;
    float m_MutationRate = 0.05f;
    float m_MutationMag = 0.05f;
    float m_Elitism = 0.2f;
};

}  // namespace gen
