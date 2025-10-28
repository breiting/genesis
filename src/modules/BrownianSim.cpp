#include <imgui.h>

#include <genesis/core/Math.hpp>
#include <genesis/modules/BrownianSim.hpp>

#include "genesis/model/SimpleEmbodiment.hpp"

namespace gen {

BrownianSim::BrownianSim() = default;

void BrownianSim::OnAttach() {
    m_View.Init();
    Reset();
}

void BrownianSim::Reset() {
    m_Particles.clear();
    m_Particles.resize(m_NumParticles);
    for (auto& p : m_Particles) {
        auto pos = RandInCircle(m_WorldHalfSize.x);
        auto particle = std::make_unique<Agent>("P");
        auto embodiment = std::make_unique<SimpleEmbodiment>(pos);
        particle->SetEmbodiment(std::move(embodiment));
        p = std::move(particle);
    }
    m_AccumTime = 0.0;
    m_TotalTime = 0.0;
    m_MeanSquareDisp = 0.0f;
    m_MeanPos = glm::vec2(0.0f);
}

void BrownianSim::Update(double dt) {
    m_AccumTime += dt;
    m_TotalTime += dt;

    for (auto& p : m_Particles) {
        StepParticle(*p, static_cast<float>(dt));
        WrapToWorld(*p);
    }

    // Statistik (MSD, mean position)
    glm::vec2 mean{0.0f};
    for (auto& p : m_Particles) mean += p->GetEmbodiment()->GetPosition();
    mean /= (float)m_Particles.size();

    double sumsq = 0.0;
    for (auto& p : m_Particles) sumsq += glm::length(p->GetEmbodiment()->GetPosition() - mean);

    m_MeanPos = mean;
    m_MeanSquareDisp = static_cast<float>(sumsq / m_Particles.size());
    m_View.UpdateInstances(m_Particles);
}

void BrownianSim::StepParticle(Agent& p, float dt) {
    glm::vec2 n = RandUnitVec2();
    auto emb = p.GetEmbodiment();
    if (!emb) return;
    emb->SetVelocity(emb->GetVelocity() * m_Damping + n * m_StepSize * dt);
    p.Update(dt);
}

void BrownianSim::WrapToWorld(Agent& agent) {
    auto emb = agent.GetEmbodiment();
    if (!emb) return;

    glm::vec2 pos = emb->GetPosition();
    glm::vec2 vel = emb->GetVelocity();

    if (m_WrapBounds) {
        // wrapping — teleport von einer Seite zur anderen
        if (pos.x > m_WorldHalfSize.x) pos.x -= 2.0f * m_WorldHalfSize.x;
        if (pos.x < -m_WorldHalfSize.x) pos.x += 2.0f * m_WorldHalfSize.x;
        if (pos.y > m_WorldHalfSize.y) pos.y -= 2.0f * m_WorldHalfSize.y;
        if (pos.y < -m_WorldHalfSize.y) pos.y += 2.0f * m_WorldHalfSize.y;
    } else {
        // bouncing — reflektiere Velocity an den Rändern
        if (pos.x > m_WorldHalfSize.x || pos.x < -m_WorldHalfSize.x) {
            vel.x *= -1.0f;
        }
        if (pos.y > m_WorldHalfSize.y || pos.y < -m_WorldHalfSize.y) {
            vel.y *= -1.0f;
        }

        // Clamp Position ins World-AABB
        pos = glm::clamp(pos, -m_WorldHalfSize, m_WorldHalfSize);
    }

    emb->SetPosition(pos);
    emb->SetVelocity(vel);
}

void BrownianSim::RenderWorld(const glm::mat4& vp) {
    m_View.Draw(vp);
}

void BrownianSim::RenderImGui() {
    ImGui::TextUnformatted("Brownian Motion Simulation");
    ImGui::Separator();
    if (ImGui::SliderInt("Particles", &m_NumParticles, 10, 5000)) Reset();
    ImGui::SliderFloat("Step Size", &m_StepSize, 0.1f, 10.0f);
    ImGui::SliderFloat("Damping", &m_Damping, 0.8f, 1.0f);
    ImGui::Checkbox("Wrap Bounds", &m_WrapBounds);
    if (ImGui::Button("Reset")) Reset();

    ImGui::Separator();
    ImGui::Text("Mean Position: (%.2f, %.2f)", m_MeanPos.x, m_MeanPos.y);
    ImGui::Text("Mean Sq. Disp: %.4f", m_MeanSquareDisp);
    ImGui::Text("Sim Time: %.2fs", m_TotalTime);
}

}  // namespace gen
