#pragma once

namespace gen {

class Agent;

/**
 * @brief Base interface for agent behaviors.
 */
class IBehavior {
   public:
    virtual ~IBehavior() = default;

    /// Execute behavior logic for one time step.
    virtual void Execute(Agent& agent, float dt) = 0;
};

}  // namespace gen
