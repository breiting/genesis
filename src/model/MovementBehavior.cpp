#include <genesis/model/Agent.hpp>
#include <genesis/model/MovementBehavior.hpp>
#include <genesis/model/NeuralNetwork.hpp>
#include <glm/geometric.hpp>

namespace gen {

MovementBehavior::MovementBehavior(glm::vec2 goal) : m_Goal(goal) {
}

void MovementBehavior::Execute(const std::string& capability, Agent& agent, float dt) {
    auto cap = agent.GetCapability(capability);
    if (!cap) return;

    auto emb = agent.GetEmbodiment();
    if (!emb) return;

    glm::vec2 toGoal = m_Goal - emb->GetPosition();
    float dist = glm::length(toGoal);
    glm::vec2 dir = dist > 0 ? glm::normalize(toGoal) : glm::vec2(0);

    float normalizedDistance = dist / glm::length(m_Goal - emb->GetStartPosition());

    std::vector<float> input = {dir.x, dir.y, emb->GetVelocity().x, emb->GetVelocity().y, normalizedDistance};
    auto output = cap->GetBrain().Forward(input);

    glm::vec2 accel(output[0], output[1]);
    float accelScale = 5.0f;
    glm::vec2 vel = emb->GetVelocity() + accel * accelScale * dt;
    float maxSpeed = 2.0f;
    if (glm::length(vel) > maxSpeed) vel = glm::normalize(vel) * maxSpeed;

    emb->SetVelocity(vel);
}

}  // namespace gen
