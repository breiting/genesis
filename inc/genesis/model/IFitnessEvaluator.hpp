#pragma once
#include "Agent.hpp"

namespace gen {

/**
 * @brief Interface for evaluating the fitness of an agent.
 */
class IFitnessEvaluator {
   public:
    virtual ~IFitnessEvaluator() = default;
    virtual float Evaluate(Agent& agent) = 0;
};

}  // namespace gen
