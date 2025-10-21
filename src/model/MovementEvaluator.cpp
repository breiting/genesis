#include <genesis/model/MovementEvaluator.hpp>
#include <glm/glm.hpp>

#include "glm/geometric.hpp"

namespace gen {

float MovementEvaluator::Evaluate(Agent& agent) {
    auto* emb = agent.GetEmbodiment();
    if (!emb) return 0.0f;

    const glm::vec2 startPos = emb->GetStartPosition();
    const glm::vec2 curPos = emb->GetPosition();

    float startDist = glm::length(startPos - m_Target);

    float fitness = 1 / startDist * glm::length(curPos - startPos);

    return fitness;

    // Higher fitness for being closer to target
    // float fitness = 1.0f / (1.0f + dist * dist);
    // agent.SetFitness(fitness);
    // return fitness;
}

}  // namespace gen
