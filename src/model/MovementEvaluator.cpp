#include <genesis/model/MovementEvaluator.hpp>
#include <glm/glm.hpp>

#include "glm/geometric.hpp"

namespace gen {

float MovementEvaluator::Evaluate(Agent& agent) {
    auto* emb = agent.GetEmbodiment();
    if (!emb) return 0.0f;

    const glm::vec2 src = emb->GetStartPosition();
    const glm::vec2 pos = emb->GetPosition();

    float startDist = glm::length(src - m_Target);
    float endDist = glm::length(pos - m_Target);

    // wie viel näher ist der Agent gekommen?
    float progress = glm::clamp((startDist - endDist) / startDist, 0.0f, 1.0f);

    // Bonus, wenn nah am Ziel
    float bonus = 0.0f;
    if (endDist < 2.0f) bonus = 1.0f - (endDist / 2.0f);  // max +1

    float fitness = glm::clamp(progress + bonus, 0.0f, 1.0f);

    // Max Bonus, wenn direkt dort
    float velMag = glm::length(emb->GetVelocity());
    if (endDist < 1.0f) {
        fitness += 0.1f * (1.0f - glm::clamp(velMag, 0.0f, 1.0f));  // Bonus fürs Stillstehen
    }

    agent.SetFitness(fitness);
    return fitness;
}

}  // namespace gen
