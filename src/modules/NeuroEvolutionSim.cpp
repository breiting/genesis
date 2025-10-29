#include "genesis/modules/NeuroEvolutionSim.hpp"

#include <imgui.h>

#include "genesis/model/MovementBehavior.hpp"
#include "genesis/model/SimpleEmbodiment.hpp"

namespace gen {

NeuroEvolutionSim::NeuroEvolutionSim() = default;

void NeuroEvolutionSim::OnAttach() {
    // Views
    m_CanvasView.Init();
    m_AgentView.Init();
    m_StartView.Init();
    m_TargetView.Init();

    // Evaluator (auf aktuelles Target)
    // m_Evaluator = std::make_unique<MovementEvaluator>(m_TargetPos);
    // Erstpopulation
    // RebuildPopulation();
}

void NeuroEvolutionSim::Reset() {
    m_Trainer.reset();
    m_Evaluator = std::make_unique<MovementEvaluator>(m_TargetPos);
    m_GenerationCount = 0;
    RebuildPopulation();
}

void NeuroEvolutionSim::RebuildPopulation() {
    m_Agents.clear();
    m_Agents.reserve(m_NumAgents);
    for (int i = 0; i < m_NumAgents; ++i) {
        CreateAgent(m_StartPos, m_TargetPos);
    }
    m_AgentView.UpdateInstances(m_Agents);
}

void NeuroEvolutionSim::CreateAgent(const glm::vec2& pos, const glm::vec2& target) {
    auto agent = std::make_unique<Agent>("agent");

    auto embodiment = std::make_unique<SimpleEmbodiment>(pos);
    agent->SetEmbodiment(std::move(embodiment));

    // Inputs: dir.x, dir.y, vel.x, vel.y, normalizedDistance
    std::vector<int> layout{5, 8, 2};
    auto cap = std::make_unique<Capability>("movement", layout, std::make_unique<MovementBehavior>(target));
    cap->Randomize();
    agent->AddCapability(std::move(cap));

    m_Agents.emplace_back(std::move(agent));
}

void NeuroEvolutionSim::RepositionAgents(bool random) {
    for (auto& a : m_Agents) {
        auto emb = a->GetEmbodiment();
        if (!emb) continue;

        glm::vec2 pos = m_StartPos;
        if (random) {
            glm::vec2 offset = RandUnitVec2() * RandFloat(0.0f, 5.0f);
            pos += offset;
        }
        emb->SetPosition(pos);
        emb->SetVelocity(glm::vec2(0.0f));
    }
}

void NeuroEvolutionSim::TrainOneGeneration() {
    if (!m_Trainer) {
        m_Trainer = std::make_unique<Trainer>(m_Agents, std::make_unique<MovementEvaluator>(m_TargetPos), "movement");
    }

    const bool useRandomStarts = (m_GenerationCount >= m_FixedStartGens);
    RepositionAgents(useRandomStarts);

    m_Trainer->RunGeneration(m_dt, m_StepsPerEpisode, m_MutationRate, m_MutationMag, m_Elitism);
    m_AgentView.UpdateInstances(m_Agents);
    ++m_GenerationCount;
}

void NeuroEvolutionSim::Update(double dt) {
    if (m_IsObserving) {
        for (auto& a : m_Agents) a->Update(dt * m_Timescale);
        m_AgentView.UpdateInstances(m_Agents);
        return;
    }

    // Trainings-“Burst”
    for (int g = 0; g < m_GenerationsPerClick; ++g) {
        TrainOneGeneration();
    }
    // After training
    RepositionAgents(false);
    m_IsObserving = true;
}

void NeuroEvolutionSim::RenderWorld(const glm::mat4& vp) {
    m_CanvasView.Draw(m_WorldSize, vp);
    m_StartView.Draw(m_StartPos, 2.0f, {1, 1, 0, 1}, vp);
    m_TargetView.Draw(m_TargetPos, 2.0f, {0, 1, 1, 1}, vp);
    m_AgentView.Draw(vp);
}

bool NeuroEvolutionSim::SaveBestGenome(const std::string& path) {
    if (!m_Trainer) return false;
    const auto& best = m_Trainer->GetBestGenome();
    return best.SaveBinary(path.c_str());
}

bool NeuroEvolutionSim::LoadGenome(const std::string& path, Genome& out) {
    return out.LoadBinary(path.c_str());
}

void NeuroEvolutionSim::DeployGenomeToAll(const Genome& g) {
    for (auto& a : m_Agents) {
        auto* cap = a->GetCapability("movement");
        if (!cap) continue;
        cap->SetFromGenome(g);
    }
    m_AgentView.UpdateInstances(m_Agents);
}

void NeuroEvolutionSim::RenderImGui() {
    ImGui::TextColored(ImVec4(0.6f, 0.9f, 0.8f, 1.0f), "NEURO-EVOLUTION");
    ImGui::Separator();

    // World / Positions
    if (ImGui::TreeNode("World & Positions")) {
        ImGui::DragFloat2("World Size", &m_WorldSize.x, 0.1f, 10.0f, 500.0f, "%.1f");
        ImGui::DragFloat2("Start Pos", &m_StartPos.x, 0.1f, -500.0f, 500.0f, "%.1f");
        ImGui::DragFloat2("Target Pos", &m_TargetPos.x, 0.1f, -500.0f, 500.0f, "%.1f");
        if (ImGui::Button("Rebuild Population")) RebuildPopulation();
        if (ImGui::Button("Randomize Agents")) RepositionAgents(true);
        ImGui::TreePop();
    }

    // Population & Episode
    if (ImGui::TreeNode("Population & Episode")) {
        if (ImGui::SliderInt("Population", &m_NumAgents, 5, 500)) {
            RebuildPopulation();
            m_Trainer.reset();
            m_GenerationCount = 0;
        }
        ImGui::SliderInt("Steps / Episode", &m_StepsPerEpisode, 60, 2000);
        ImGui::SliderFloat("dt", &m_dt, 0.01f, 0.2f, "%.3f");
        ImGui::SliderInt("Fixed Start Gens", &m_FixedStartGens, 0, 200);
        ImGui::TreePop();
    }

    // GA Params
    if (ImGui::TreeNode("Evolution Params")) {
        ImGui::SliderFloat("Mutation Rate", &m_MutationRate, 0.0f, 0.5f, "%.3f");
        ImGui::SliderFloat("Mutation Mag", &m_MutationMag, 0.0f, 0.5f, "%.3f");
        ImGui::SliderFloat("Elitism", &m_Elitism, 0.0f, 0.8f, "%.2f");
        ImGui::TreePop();
    }

    // Control
    ImGui::Separator();
    ImGui::Text("Control");
    if (ImGui::Button("Init", ImVec2(120, 28))) {
        Reset();
        m_IsObserving = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Train", ImVec2(120, 28))) {
        m_IsObserving = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Observe", ImVec2(120, 28))) {
        m_IsObserving = true;
    }

    ImGui::SliderInt("Gens/Click", &m_GenerationsPerClick, 1, 100);
    ImGui::SliderFloat("Timescale", &m_Timescale, 0.2f, 10.0f, "%.2f");

    // Stats
    if (m_Trainer) {
        ImGui::Separator();
        ImGui::Text("Generation: %d", m_GenerationCount);
        ImGui::Text("Best Fitness:   %.3f", m_Trainer->GetBestFitness());
        ImGui::Text("Average Fitness: %.3f", m_Trainer->GetAverageFitness());
        ImGui::ProgressBar(m_Trainer->GetBestFitness(), ImVec2(220, 16));
    }

    // Genome IO
    ImGui::Separator();
    ImGui::Text("Genome");
    ImGui::InputText("Path", m_LastGenomePath.data(), m_LastGenomePath.capacity() + 1);
    if (ImGui::Button("Save Best", ImVec2(110, 24))) {
        if (m_Trainer) {
            if (SaveBestGenome(m_LastGenomePath)) { /* ok */
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Load", ImVec2(80, 24))) {
        Genome g;
        if (LoadGenome(m_LastGenomePath, g)) {
            m_BestGenome = g;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Deploy", ImVec2(90, 24))) {
        DeployGenomeToAll(m_BestGenome);
        RepositionAgents(true);
        m_IsObserving = true;
    }
}

}  // namespace gen
