#include <genesis/model/agent/Agent.hpp>
#include <genesis/model/agent/IBehavior.hpp>

namespace gen {

Agent::Agent(const std::string& id, glm::vec2 pos, float radius) : Entity(id, pos, radius) {
}

void Agent::Update(float dt) {
    if (m_Behavior) {
        m_Behavior->Execute(*this, dt);
    }
}

void Agent::SetBehavior(std::unique_ptr<IBehavior> behavior) {
    m_Behavior = std::move(behavior);
}

}  // namespace gen
