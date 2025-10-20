#pragma once
#include <glm/glm.hpp>
#include <string>

namespace gen {

/**
 * @brief Basic physical entity with position, velocity and radius.
 */
class Entity {
   public:
    explicit Entity(const std::string& id, glm::vec2 pos, float radius = 0.1f);

    /// @return Unique identifier.
    const std::string& GetId() const {
        return m_Id;
    }

    /// @return Current world-space position.
    const glm::vec2& GetPosition() const {
        return m_Pos;
    }
    void SetPosition(const glm::vec2& p) {
        m_Pos = p;
    }

    /// @return Current velocity vector.
    const glm::vec2& GetVelocity() const {
        return m_Vel;
    }
    void SetVelocity(const glm::vec2& v) {
        m_Vel = v;
    }

    /// @return Radius of the entity.
    float GetRadius() const {
        return m_Radius;
    }

   protected:
    std::string m_Id;
    glm::vec2 m_Pos;
    glm::vec2 m_Vel;
    float m_Radius;
};

}  // namespace gen
