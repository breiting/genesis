#pragma once

#include <genesis/model/agent/Agent.hpp>
#include <vector>

namespace gen {

/**
 * @brief Environment that evaluates agents trying to reach a goal position.
 */
class PathEnvironment {
   public:
    PathEnvironment(size_t numAgents, glm::vec2 worldMin = {-10.0f, -10.0f}, glm::vec2 worldMax = {10.0f, 10.0f});

    void Reset();

    /// Run one episode with the given network (returns average fitness).
    float RunEpisode(NeuralNetwork& nn, size_t steps = 300, float dt = 0.1f);

    const std::vector<Agent>& GetAgents() const {
        return m_Agents;
    }
    glm::vec2 GetGoal() const {
        return m_Goal;
    }

   private:
    glm::vec2 m_WorldMin;
    glm::vec2 m_WorldMax;
    glm::vec2 m_Start;
    glm::vec2 m_Goal;
    std::vector<Agent> m_Agents;
};

}  // namespace gen
