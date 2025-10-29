#pragma once
#include <genesis/model/Agent.hpp>
#include <genesis/modules/ISimModule.hpp>
#include <genesis/view/AgentView.hpp>
#include <genesis/view/Camera2D.hpp>
#include <genesis/view/CanvasView.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace gen {

class SwarmSim : public ISimModule {
   public:
    SwarmSim();

    std::string Name() const override {
        return "SwarmSim";
    }
    void OnAttach() override;
    void OnDetach() override {
    }
    void Reset() override;
    void Update(double dt) override;
    void RenderWorld(const glm::mat4& vp) override;
    void RenderImGui() override;

   private:
    void ApplySwarmRules(Agent& agent, double dt);
    std::vector<Agent*> GetNeighbors(Agent& agent);

   private:
    std::vector<std::unique_ptr<Agent>> m_Agents;
    AgentView m_AgentView;
    CanvasView m_CanvasView;

    glm::vec2 m_WorldSize{100.0f, 100.0f};
    float m_MaxSpeed = 10.0f;
    float m_NeighborRadius = 15.0f;

    float m_WeightSeparation = 1.2f;
    float m_WeightAlignment = 1.0f;
    float m_WeightCohesion = 1.0f;

    int m_NumAgents = 300;
    bool m_ShowNeighborhood = false;
};

}  // namespace gen
