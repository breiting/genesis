#include <genesis/model/agent/MovementBehavior.hpp>
#include <genesis/model/environment/PathEnvironment.hpp>
#include <genesis/model/evolution/NeuralNetwork.hpp>

namespace gen {

PathEnvironment::PathEnvironment(size_t numAgents, glm::vec2 worldMin, glm::vec2 worldMax)
    : m_WorldMin(worldMin), m_WorldMax(worldMax) {
    m_Agents.reserve(numAgents);
    for (size_t i = 0; i < numAgents; ++i) m_Agents.emplace_back("A" + std::to_string(i), glm::vec2(0.0f));
    Reset();
}

void PathEnvironment::Reset() {
    m_Start = {RandFloat(m_WorldMin.x, m_WorldMax.x), RandFloat(m_WorldMin.y, m_WorldMax.y)};
    m_Goal = {RandFloat(m_WorldMin.x, m_WorldMax.x), RandFloat(m_WorldMin.y, m_WorldMax.y)};
    for (auto& a : m_Agents) {
        a.SetPosition(m_Start);
        a.SetVelocity({0, 0});
    }
}

float PathEnvironment::RunEpisode(NeuralNetwork& nn, size_t steps, float dt) {
    Reset();
    for (auto& a : m_Agents) {
        a.SetBrain(std::make_shared<NeuralNetwork>(nn));  // clone brain
    }

    float totalFitness = 0.0f;
    for (size_t step = 0; step < steps; ++step) {
        for (auto& agent : m_Agents) {
            MovementBehavior behavior(m_Goal);
            agent.SetBehavior(std::make_unique<MovementBehavior>(behavior));
            agent.Update(dt);

            float dist = glm::length(agent.GetPosition() - m_Goal);
            totalFitness += 1.0f / (1.0f + dist);

            if (dist < 0.5f) totalFitness += 10.0f;  // bonus for reaching goal
        }
    }
    return totalFitness / static_cast<float>(m_Agents.size());
}

}  // namespace gen
