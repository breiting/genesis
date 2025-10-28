#include <imgui.h>

#include <genesis/modules/NeuroEvolutionSim.hpp>

#include "genesis/model/MovementBehavior.hpp"
#include "genesis/model/SimpleEmbodiment.hpp"

namespace gen {

NeuroEvolutionSim::NeuroEvolutionSim() = default;

void NeuroEvolutionSim::OnAttach() {
    m_CanvasView.Init();
    m_AgentView.Init();
    m_StartView.Init();
    m_TargetView.Init();

    m_Evaluator = std::make_unique<MovementEvaluator>(m_TargetPos);

    // create initial population
    for (int i = 0; i < m_NumAgents; ++i) CreateAgent(m_StartPos, m_TargetPos);
}

void NeuroEvolutionSim::CreateAgent(const glm::vec2& pos, const glm::vec2& target) {
    auto agent = std::make_unique<Agent>("agent");

    auto embodiment = std::make_unique<SimpleEmbodiment>(pos);
    agent->SetEmbodiment(std::move(embodiment));

    std::vector<int> layout{5, 8, 2};
    auto cap = std::make_unique<Capability>("movement", layout, std::make_unique<MovementBehavior>(target));
    cap->Randomize();
    agent->AddCapability(std::move(cap));

    m_Agents.emplace_back(std::move(agent));
}

void NeuroEvolutionSim::Reset() {
    m_Agents.clear();
    m_Trainer.reset();
    m_Evaluator = std::make_unique<MovementEvaluator>(m_TargetPos);
    m_GenerationCount = 0;

    for (int i = 0; i < m_NumAgents; ++i) CreateAgent(m_StartPos, m_TargetPos);
}

void NeuroEvolutionSim::RepositionAgents(bool random) {
    for (auto& a : m_Agents) {
        glm::vec2 offset = random ? RandUnitVec2() * RandFloat(0.0f, 5.0f) : glm::vec2(0.0f);
        auto emb = a->GetEmbodiment();
        emb->SetPosition(m_StartPos + offset);
        emb->SetVelocity(glm::vec2(0.0f));
    }
}

void NeuroEvolutionSim::TrainGeneration() {
    if (!m_Trainer) {
        m_Trainer = std::make_unique<Trainer>(m_Agents, std::make_unique<MovementEvaluator>(m_TargetPos), "movement");
    }

    constexpr float dt = 0.1f;
    constexpr int steps = 500;

    RepositionAgents(true);
    m_Trainer->RunGeneration(dt, steps, m_MutationRate, m_MutationMag, m_Elitism);
    m_AgentView.UpdateInstances(m_Agents);

    ++m_GenerationCount;
}

void NeuroEvolutionSim::Update(double dt) {
    if (m_IsObserving) {
        for (auto& a : m_Agents) a->Update(dt * m_Timescale);
        m_AgentView.UpdateInstances(m_Agents);
    } else {
        TrainGeneration();
    }
}

void NeuroEvolutionSim::RenderWorld(const glm::mat4& vp) {
    m_CanvasView.Draw(m_WorldSize, vp);
    m_StartView.Draw(m_StartPos, 2.0f, {1, 1, 0, 1}, vp);
    m_TargetView.Draw(m_TargetPos, 2.0f, {0, 1, 1, 1}, vp);
    m_AgentView.Draw(vp);
}

void NeuroEvolutionSim::RenderImGui() {
    ImGui::TextColored(ImVec4(0.6f, 0.9f, 0.8f, 1.0f), "NEURO-EVOLUTION");
    ImGui::Separator();
    ImGui::SliderInt("Population", &m_NumAgents, 10, 200);
    ImGui::SliderFloat("Mutation Rate", &m_MutationRate, 0.0f, 0.3f, "%.3f");
    ImGui::SliderFloat("Mutation Mag", &m_MutationMag, 0.0f, 0.3f, "%.3f");
    ImGui::SliderFloat("Elitism", &m_Elitism, 0.0f, 0.5f, "%.2f");

    if (ImGui::Button("Init")) Reset();
    ImGui::SameLine();
    if (ImGui::Button("Train")) m_IsObserving = false;
    ImGui::SameLine();
    if (ImGui::Button("Observe")) m_IsObserving = true;

    if (m_Trainer) {
        ImGui::Separator();
        ImGui::Text("Generation: %d", m_GenerationCount);
        ImGui::Text("Best Fitness: %.3f", m_Trainer->GetBestFitness());
        ImGui::Text("Average Fitness: %.3f", m_Trainer->GetAverageFitness());
        ImGui::ProgressBar(m_Trainer->GetBestFitness(), ImVec2(200, 16));
    }
}
}  // namespace gen
