#pragma once
#include <genesis/modules/ISimModule.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "genesis/view/AgentView.hpp"

namespace gen {

class BrownianSim : public ISimModule {
   public:
    BrownianSim();
    ~BrownianSim() override = default;

    std::string Name() const override {
        return "Brownian Motion";
    }

    void OnAttach() override;
    void Reset() override;
    void Update(double dt) override;
    void RenderWorld(const glm::mat4& vp) override;
    void RenderImGui() override;

   private:
    void StepParticle(Agent& p, float dt);
    void WrapToWorld(Agent& p);

   private:
    std::vector<std::unique_ptr<Agent>> m_Particles;
    AgentView m_View;

    // world bounds
    glm::vec2 m_WorldHalfSize{5.0f, 5.0f};

    // simulation params
    int m_NumParticles = 500;
    float m_StepSize = 5.0f;   // velocity magnitude per step
    float m_Damping = 0.92f;   // velocity persistence
    bool m_WrapBounds = true;  // wrap or bounce

    // colors
    glm::vec4 m_Color = {0.56f, 0.70f, 0.76f, 1.0f};  // nord bright blue

    // stats
    double m_AccumTime = 0.0;
    double m_TotalTime = 0.0;
    float m_MeanSquareDisp = 0.0f;
    glm::vec2 m_MeanPos{0.0f};
};

}  // namespace gen
