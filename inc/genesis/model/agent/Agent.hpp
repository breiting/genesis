#pragma once
#include <memory>

#include "Entity.hpp"

namespace gen {

class IBehavior;
class NeuralNetwork;

/**
 * @brief Cognitive agent that can act based on a neural policy (brain).
 */
class Agent : public Entity {
   public:
    explicit Agent(const std::string& id, glm::vec2 pos, float radius = 0.1f);

    /// Update the agent according to its current behavior.
    void Update(float dt);

    /// Assign the agent's behavior (policy layer).
    void SetBehavior(std::unique_ptr<IBehavior> behavior);

    /// @return Pointer to current behavior.
    IBehavior* GetBehavior() const {
        return m_Behavior.get();
    }

    /// Assign the neural network controlling the behavior.
    void SetBrain(std::shared_ptr<NeuralNetwork> nn) {
        m_Brain = std::move(nn);
    }

    /// @return Pointer to brain network.
    NeuralNetwork* GetBrain() const {
        return m_Brain.get();
    }

   private:
    std::unique_ptr<IBehavior> m_Behavior;
    std::shared_ptr<NeuralNetwork> m_Brain;
};

}  // namespace gen
