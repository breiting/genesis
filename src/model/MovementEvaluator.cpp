#include <genesis/model/MovementEvaluator.hpp>
#include <glm/glm.hpp>

#include "glm/geometric.hpp"

namespace gen {

float MovementEvaluator::Evaluate(Agent& agent) {
    auto* emb = agent.GetEmbodiment();
    if (!emb) return 0.0f;

    const glm::vec2 src = emb->GetStartPosition();  // start position
    const glm::vec2 pos = emb->GetPosition();       // current position

    float dSrc = glm::length(src - m_Target);
    float dPos = glm::length(pos - m_Target);

    float r = dPos / dSrc;  // normalize to [0..1]

    float fitness = 1 / (1 + r);

    return fitness;

    // Higher fitness for being closer to target
    // float fitness = 1.0f / (1.0f + dist * dist);
    // agent.SetFitness(fitness);
    // return fitness;
}

}  // namespace gen
