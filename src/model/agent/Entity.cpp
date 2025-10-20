#include <genesis/model/agent/Entity.hpp>

namespace gen {

Entity::Entity(const std::string& id, glm::vec2 pos, float radius) : m_Id(id), m_Pos(pos), m_Radius(radius) {
}

}  // namespace gen
