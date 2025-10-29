#include <imgui.h>

#include <fstream>
#include <genesis/core/Math.hpp>
#include <genesis/model/Agent.hpp>
#include <genesis/model/Capability.hpp>
#include <genesis/model/SimpleEmbodiment.hpp>
#include <genesis/modules/RegressionSim.hpp>
#include <iostream>
#include <sstream>

namespace gen {

RegressionSim::RegressionSim() = default;

std::string RegressionSim::Name() const {
    return "RegressionSim";
}

void RegressionSim::OnAttach() {
    Reset();
}

void RegressionSim::OnDetach() {
    // Cleanup if needed.
}

void RegressionSim::Reset() {
    LoadData(m_DataPath, m_WindowSize);
    RebuildPopulation();
    m_Trainer.reset();
    m_Evaluator = std::make_unique<RegressionEvaluator>(m_Inputs, m_Targets);
}

void RegressionSim::LoadData(const std::string& filename, size_t windowSize) {
    m_Inputs.clear();
    m_Targets.clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open data file: " << filename << std::endl;
        return;
    }

    std::vector<float> prices;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        float price;
        if (ss >> price) {
            prices.push_back(price);
        }
    }

    // Create sliding windows. ( with z-scoring)
    for (size_t i = 0; i + windowSize < prices.size(); ++i) {
        std::vector<float> input(prices.begin() + i, prices.begin() + i + windowSize);

        // --- Z-Score Normalization ---
        float mean = 0.0f;
        for (float v : input) mean += v;
        mean /= input.size();

        float stddev = 0.0f;
        for (float v : input) stddev += (v - mean) * (v - mean);
        stddev = std::sqrt(stddev / input.size());
        if (stddev < 1e-6f) stddev = 1.0f;  // prevent div-by-zero

        for (auto& v : input) v = (v - mean) / stddev;

        // --- Target als Return (nächste Preisänderung relativ zum letzten) ---
        float next = prices[i + windowSize];
        float last = prices[i + windowSize - 1];
        float target = (next - last) / last;  // relativer Return (~[-0.05, 0.05])

        m_Inputs.push_back(input);
        m_Targets.push_back(target);
    }

    std::cout << "Loaded " << m_Inputs.size() << " regression samples with outputs " << m_Targets.size() << "."
              << std::endl;
}

void RegressionSim::RebuildPopulation() {
    m_Agents.clear();
    m_Agents.reserve(m_NumAgents);
    for (int i = 0; i < m_NumAgents; ++i) {
        auto agent = std::make_unique<Agent>("regressor");

        // Neural network layout: windowSize inputs, hidden layers, 1 output.
        std::vector<int> layout{static_cast<int>(m_WindowSize), 32, 16, 1};
        auto cap = std::make_unique<Capability>("regression", layout, nullptr);  // No behavior needed.
        auto brain = cap->GetBrain();
        brain.SetActivation(brain.GetLayerCount() - 1, ActivationType::Linear);
        cap->Randomize();
        agent->AddCapability(std::move(cap));

        m_Agents.emplace_back(std::move(agent));
    }
}

void RegressionSim::TrainOneGeneration() {
    if (!m_Trainer) {
        m_Trainer = std::make_unique<Trainer>(m_Agents, std::move(m_Evaluator), "regression");
    }

    m_Trainer->RunGeneration(0.0f, 1, m_MutationRate, m_MutationMag, m_Elitism);  // dt=0, steps=1 since no simulation.
}

void RegressionSim::Update(double /*dt*/) {
    if (m_IsTraining) {
        for (int g = 0; g < m_GenerationsPerClick; ++g) {
            TrainOneGeneration();
        }
    }
}

void RegressionSim::RenderWorld(const glm::mat4& /*vp*/) {
    // No world rendering for regression; it's data-driven.
}

void RegressionSim::RenderImGui() {
    ImGui::TextColored(ImVec4(0.6f, 0.9f, 0.8f, 1.0f), "REGRESSION SIMULATION");
    ImGui::Separator();

    // Data settings
    if (ImGui::TreeNode("Data Settings")) {
        ImGui::InputText("Data Path", m_DataPath.data(), m_DataPath.capacity() + 1);
        if (ImGui::SliderInt("Window Size", reinterpret_cast<int*>(&m_WindowSize), 1, 20)) {
            Reset();
        }
        if (ImGui::Button("Reload Data")) {
            Reset();
        }
        ImGui::TreePop();
    }

    // Population & Evolution
    if (ImGui::TreeNode("Population & Evolution")) {
        if (ImGui::SliderInt("Population", &m_NumAgents, 5, 500)) {
            RebuildPopulation();
            m_Trainer.reset();
        }
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
        m_IsTraining = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Train", ImVec2(120, 28))) {
        m_IsTraining = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop", ImVec2(120, 28))) {
        m_IsTraining = false;
    }

    ImGui::SliderInt("Gens/Click", &m_GenerationsPerClick, 1, 100);

    // Stats
    if (m_Trainer) {
        ImGui::Separator();
        ImGui::Text("Generation: %d", m_Trainer->GetGenerationCount());
        ImGui::Text("Best Fitness: %.6f", m_Trainer->GetBestFitness());
        ImGui::Text("Average Fitness: %.6f", m_Trainer->GetAverageFitness());
        ImGui::ProgressBar(m_Trainer->GetBestFitness(), ImVec2(220, 16));

        // Plot predictions vs actuals for best agent
        if (ImGui::TreeNode("Prediction Plot")) {
            auto* bestAgent = m_Agents[0].get();
            auto* cap = bestAgent->GetCapability("regression");
            if (cap && !m_Targets.empty()) {
                static std::vector<float> predictions;
                predictions.clear();
                for (const auto& input : m_Inputs) {
                    auto pred = cap->GetBrain().Forward(input);
                    predictions.push_back(pred[0]);
                }

                ImGui::PlotLines("Actual", m_Targets.data(), m_Targets.size(), 0, nullptr, FLT_MAX, FLT_MAX,
                                 ImVec2(400, 200));
                ImGui::PlotLines("Predicted", predictions.data(), predictions.size(), 0, nullptr, FLT_MAX, FLT_MAX,
                                 ImVec2(400, 200));
            }
            ImGui::TreePop();
        }
    }
}

}  // namespace gen
