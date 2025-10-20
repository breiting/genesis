#pragma once
#include <genesis/model/agent/Agent.hpp>
#include <vector>

namespace gen {

/**
 * @brief A population of agents evolving over time.
 */
class Population {
   public:
    Population(size_t size, const std::vector<int>& brainLayout);

    /// Reset all agents with random brains.
    void Initialize();

    /// Run one simulation epoch
    void Simulate(float dt, size_t steps);

    /// Apply simple evolutionary step (sort + mutate).
    void Evolve(float mutationRate, float mutationMag, float elitism = 0.1f);

    /// @return All agents.
    std::vector<std::unique_ptr<Agent>>& GetAgents() {
        return m_Agents;
    }

    /// @return Best agent by fitness.
    const Agent& GetBest() const;

   private:
    std::vector<std::unique_ptr<Agent>> m_Agents;
    std::vector<int> m_BrainLayout;
};

}  // namespace gen
