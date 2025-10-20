#pragma once
#include <genesis/model/IBehavior.hpp>
#include <glm/glm.hpp>

namespace gen {

/**
 * @brief Simple goal-oriented movement behavior driven by a neural network.
 */
class MovementBehavior : public IBehavior {
   public:
    explicit MovementBehavior(glm::vec2 goal);

    void Execute(const std::string& capability, Agent& agent, float dt) override;

   private:
    glm::vec2 m_Goal;
};

}  // namespace gen
