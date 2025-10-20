#pragma once
#include <genesis/model/agent/Agent.hpp>
#include <genesis/model/evolution/Population.hpp>
#include <vector>

namespace gen {

class PathEnvironment {
   public:
    PathEnvironment(glm::vec2 worldMin = {-10.0f, -10.0f}, glm::vec2 worldMax = {10.0f, 10.0f});

    void Reset(size_t numAgents);

    /// Run one full episode for all genomes in the population.
    void RunEpisode(Population& pop, const std::vector<int>& layout, size_t steps = 300, float dt = 0.1f);

   private:
    glm::vec2 m_WorldMin;
    glm::vec2 m_WorldMax;
    glm::vec2 m_Goal;
    std::vector<Agent> m_Agents;
};

}  // namespace gen
