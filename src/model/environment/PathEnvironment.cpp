#include <genesis/model/agent/MovementBehavior.hpp>
#include <genesis/model/environment/PathEnvironment.hpp>
#include <genesis/model/evolution/NeuralNetwork.hpp>

namespace gen {

PathEnvironment::PathEnvironment(glm::vec2 worldMin, glm::vec2 worldMax) : m_WorldMin(worldMin), m_WorldMax(worldMax) {
}

void PathEnvironment::Reset(size_t numAgents) {
    m_Agents.clear();
    for (size_t i = 0; i < numAgents; ++i) {
        glm::vec2 start = {RandFloat(m_WorldMin.x, m_WorldMax.x), RandFloat(m_WorldMin.y, m_WorldMax.y)};
        m_Agents.emplace_back("A" + std::to_string(i), start);
    }

    m_Goal = {RandFloat(m_WorldMin.x, m_WorldMax.x), RandFloat(m_WorldMin.y, m_WorldMax.y)};
}

void PathEnvironment::RunEpisode(Population& pop, const std::vector<int>& layout, size_t steps, float dt) {
    size_t numAgents = pop.Size();
    Reset(numAgents);

    for (size_t i = 0; i < numAgents; ++i) {
        auto& genome = pop.GetGenomes()[i];
        auto nn = std::make_shared<NeuralNetwork>(layout);
        nn->SetWeightsAndBiases(genome.GetDNA());
        // m_Agents[i].SetBrain(nn);
        // m_Agents[i].SetBehavior(std::make_unique<MovementBehavior>(m_Goal));
    }

    for (size_t step = 0; step < steps; ++step) {
        for (auto& agent : m_Agents) agent.Update(dt);
    }

    for (size_t i = 0; i < numAgents; ++i) {
        float startDist = glm::length(m_Agents[i].GetPosition() - m_Goal);
        float dist = glm::length(m_Agents[i].GetPosition() - m_Goal);
        float fitness = 1.0f - std::clamp(dist / (startDist + 1e-6f), 0.0f, 1.0f);
        pop.GetGenomes()[i].SetFitness(fitness);
    }
}

}  // namespace gen
