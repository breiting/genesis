#include <imgui.h>

#include <genesis/core/Math.hpp>
#include <genesis/model/SimpleEmbodiment.hpp>
#include <genesis/modules/SwarmSim.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>

namespace gen {

SwarmSim::SwarmSim() = default;

void SwarmSim::OnAttach() {
    m_CanvasView.Init();
    m_AgentView.Init();
    Reset();
}

void SwarmSim::Reset() {
    m_Agents.clear();
    m_Agents.reserve(m_NumAgents);

    for (int i = 0; i < m_NumAgents; ++i) {
        auto agent = std::make_unique<Agent>("boid");
        glm::vec2 pos = RandInCircle(m_WorldSize.x * 0.5f);
        glm::vec2 vel = RandUnitVec2() * RandFloat(2.0f, 5.0f);
        auto embodiment = std::make_unique<SimpleEmbodiment>(pos);
        embodiment->SetVelocity(vel);
        embodiment->SetRadius(0.5);
        agent->SetEmbodiment(std::move(embodiment));
        m_Agents.emplace_back(std::move(agent));
    }

    m_AgentView.UpdateInstances(m_Agents);
}

void SwarmSim::Update(double dt) {
    for (auto& agent : m_Agents) {
        ApplySwarmRules(*agent, dt);
        agent->Update(dt);
    }

    // Wrap to world bounds
    for (auto& agent : m_Agents) {
        auto emb = agent->GetEmbodiment();
        auto pos = emb->GetPosition();

        if (pos.x > m_WorldSize.x / 2) pos.x -= m_WorldSize.x;
        if (pos.x < -m_WorldSize.x / 2) pos.x += m_WorldSize.x;
        if (pos.y > m_WorldSize.y / 2) pos.y -= m_WorldSize.y;
        if (pos.y < -m_WorldSize.y / 2) pos.y += m_WorldSize.y;

        emb->SetPosition(pos);
    }

    m_AgentView.UpdateInstances(m_Agents);
}

void SwarmSim::ApplySwarmRules(Agent& agent, double dt) {
    auto emb = agent.GetEmbodiment();
    if (!emb) return;

    glm::vec2 pos = emb->GetPosition();
    glm::vec2 vel = emb->GetVelocity();

    std::vector<Agent*> neighbors = GetNeighbors(agent);
    if (neighbors.empty()) return;

    glm::vec2 sep(0), ali(0), coh(0);
    for (auto* n : neighbors) {
        auto nEmb = n->GetEmbodiment();
        glm::vec2 toNeighbor = nEmb->GetPosition() - pos;
        float dist2 = glm::length2(toNeighbor);

        if (dist2 > 0.0001f) {
            sep -= toNeighbor / dist2;
            ali += nEmb->GetVelocity();
            coh += nEmb->GetPosition();
        }
    }

    ali /= (float)neighbors.size();
    coh = (coh / (float)neighbors.size()) - pos;

    glm::vec2 acc = m_WeightSeparation * sep + m_WeightAlignment * (ali - vel) + m_WeightCohesion * coh;

    vel += acc * (float)dt;

    float speed = glm::length(vel);
    if (speed > m_MaxSpeed) vel = glm::normalize(vel) * m_MaxSpeed;

    emb->SetVelocity(vel);
}

std::vector<Agent*> SwarmSim::GetNeighbors(Agent& agent) {
    std::vector<Agent*> neighbors;
    auto emb = agent.GetEmbodiment();
    if (!emb) return neighbors;
    auto pos = emb->GetPosition();

    for (auto& other : m_Agents) {
        if (other.get() == &agent) continue;
        auto oEmb = other->GetEmbodiment();
        float dist2 = glm::length2(oEmb->GetPosition() - pos);
        if (dist2 < m_NeighborRadius * m_NeighborRadius) neighbors.push_back(other.get());
    }
    return neighbors;
}

void SwarmSim::RenderWorld(const glm::mat4& vp) {
    m_CanvasView.Draw(m_WorldSize, vp);
    m_AgentView.Draw(vp);
}

void SwarmSim::RenderImGui() {
    ImGui::TextColored(ImVec4(0.6f, 0.9f, 0.8f, 1.0f), "SWARM SIMULATION");
    ImGui::Separator();

    if (ImGui::Button("Reset", ImVec2(100, 28))) Reset();

    ImGui::SliderInt("Num Agents", &m_NumAgents, 10, 2000);
    ImGui::SliderFloat("Neighbor Radius", &m_NeighborRadius, 5.0f, 50.0f);
    ImGui::SliderFloat("Max Speed", &m_MaxSpeed, 1.0f, 30.0f);

    ImGui::Separator();
    ImGui::Text("Weights");
    ImGui::SliderFloat("Separation", &m_WeightSeparation, 0.0f, 3.0f);
    ImGui::SliderFloat("Alignment", &m_WeightAlignment, 0.0f, 3.0f);
    ImGui::SliderFloat("Cohesion", &m_WeightCohesion, 0.0f, 3.0f);

    ImGui::Checkbox("Show Neighborhood", &m_ShowNeighborhood);
}

}  // namespace gen
