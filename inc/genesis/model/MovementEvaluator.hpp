#pragma once
#include <glm/vec2.hpp>

#include "IFitnessEvaluator.hpp"

namespace gen {

/**
 * @brief Evaluates how well an agent moves toward a target.
 * Fitness = 1 / (1 + distance_to_target)
 */
class MovementEvaluator : public IFitnessEvaluator {
   public:
    explicit MovementEvaluator(glm::vec2 target) : m_Target(target) {
    }

    float Evaluate(Agent& agent) override;

   private:
    glm::vec2 m_Target;
};

}  // namespace gen
