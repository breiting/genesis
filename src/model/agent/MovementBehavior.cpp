#include <genesis/model/agent/Agent.hpp>
#include <genesis/model/agent/MovementBehavior.hpp>
#include <genesis/model/evolution/NeuralNetwork.hpp>
#include <glm/geometric.hpp>

namespace gen {

MovementBehavior::MovementBehavior(glm::vec2 goal) : m_Goal(goal) {
}

void MovementBehavior::Execute(Agent& agent, float dt) {
    auto brain = agent.GetBrain();
    if (!brain) return;

    glm::vec2 toGoal = m_Goal - agent.GetPosition();
    float dist = glm::length(toGoal);
    glm::vec2 dir = dist > 0 ? glm::normalize(toGoal) : glm::vec2(0);

    std::vector<float> input = {dir.x, dir.y, agent.GetVelocity().x, agent.GetVelocity().y};
    auto output = brain->Forward(input);

    glm::vec2 accel(output[0], output[1]);
    glm::vec2 vel = agent.GetVelocity() + accel * dt;
    float maxSpeed = 2.0f;
    if (glm::length(vel) > maxSpeed) vel = glm::normalize(vel) * maxSpeed;

    agent.SetVelocity(vel);
    agent.SetPosition(agent.GetPosition() + vel * dt);
}

}  // namespace gen
