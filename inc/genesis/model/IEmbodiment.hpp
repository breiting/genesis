#pragma once

#include <glm/glm.hpp>

namespace gen {

/**
 * @brief Interface for optional physical embodiment of an agent.
 */
class IEmbodiment {
   public:
    virtual ~IEmbodiment() = default;

    virtual glm::vec2 GetPosition() const = 0;
    virtual void SetPosition(const glm::vec2& pos) = 0;

    virtual glm::vec2 GetVelocity() const = 0;
    virtual void SetVelocity(const glm::vec2& vel) = 0;

    virtual float GetRadius() const = 0;
    virtual void SetRadius(float radius) = 0;

    virtual void Update(float dt) = 0;
};
}  // namespace gen
