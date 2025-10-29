#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

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

    // ISimModule
    void OnAttach() override;
    void Reset() override;
    void Update(double dt) override;
    void RenderWorld(const glm::mat4& vp) override;
    void RenderImGui() override;

    void SetWorldSize(const glm::vec2& size) {
        m_WorldSize = size;
    }
    void SetStartPos(const glm::vec2& p) {
        m_StartPos = p;
        RebuildPopulation();
    }
    void SetTargetPos(const glm::vec2& p) {
        m_TargetPos = p;
        m_Evaluator = std::make_unique<MovementEvaluator>(m_TargetPos);
    }

   private:
    // Population
    void RebuildPopulation();
    void CreateAgent(const glm::vec2& pos, const glm::vec2& target);

    // Training / Observe
    void TrainOneGeneration();
    void RepositionAgents(bool random);

    // Genome IO / Deploy
    void DeployGenomeToAll(const Genome& g);
    bool SaveBestGenome(const std::string& path);
    bool LoadGenome(const std::string& path, Genome& out);

   private:
    // Data
    std::vector<std::unique_ptr<Agent>> m_Agents;
    std::unique_ptr<Trainer> m_Trainer;
    std::unique_ptr<MovementEvaluator> m_Evaluator;

    // Views
    CanvasView m_CanvasView;
    AgentView m_AgentView;
    TargetView m_StartView;
    TargetView m_TargetView;

    // World
    glm::vec2 m_WorldSize{80.0f, 80.0f};
    glm::vec2 m_StartPos{-30.0f, 0.0f};
    glm::vec2 m_TargetPos{0.0f, 0.0f};

    // Population & GA
    int m_NumAgents = 100;
    int m_GenerationCount = 0;
    int m_GenerationsPerClick = 5;  // wenn "Train" gedrückt: so viele Gens
    int m_FixedStartGens = 20;      // Curriculum: ersten N Gen fixe Starts

    float m_MutationRate = 0.05f;
    float m_MutationMag = 0.05f;
    float m_Elitism = 0.20f;

    // Episode
    float m_dt = 0.1f;
    int m_StepsPerEpisode = 500;
    float m_Timescale = 1.0f;

    // Mode
    bool m_IsObserving = false;

    // Best genome buffer
    Genome m_BestGenome;
    std::string m_LastGenomePath{"best.dna"};
};

}  // namespace gen
