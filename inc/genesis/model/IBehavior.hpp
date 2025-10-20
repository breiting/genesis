#pragma once

#include <string>
namespace gen {

class Agent;

/**
 * @brief Base interface for agent behaviors.
 */
class IBehavior {
   public:
    virtual ~IBehavior() = default;

    /// Execute behavior logic for one time step.
    virtual void Execute(const std::string& capability, Agent& agent, float dt) = 0;
};

}  // namespace gen
