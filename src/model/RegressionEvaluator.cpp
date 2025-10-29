#include <cassert>
#include <genesis/model/Agent.hpp>
#include <genesis/model/Capability.hpp>
#include <genesis/model/RegressionEvaluator.hpp>

namespace gen {

RegressionEvaluator::RegressionEvaluator(const std::vector<std::vector<float>>& inputs,
                                         const std::vector<float>& targets)
    : m_Inputs(inputs), m_Targets(targets) {
    assert(!inputs.empty() && inputs.size() == targets.size());
}

float RegressionEvaluator::Evaluate(Agent& agent) {
    auto* cap = agent.GetCapability("regression");
    if (!cap) {
        return 0.0f;  // No capability, zero fitness.
    }

    float totalError = 0.0f;
    size_t sampleCount = m_Inputs.size();

    for (size_t i = 0; i < sampleCount; ++i) {
        const auto& input = m_Inputs[i];
        float target = m_Targets[i];

        // Forward pass through the network.
        auto output = cap->GetBrain().Forward(input);
        assert(output.size() == 1);  // Single output for regression.

        float prediction = output[0];
        float error = prediction - target;
        totalError += error * error;
    }

    float mse = totalError / static_cast<float>(sampleCount);
    // Fitness: higher for lower MSE.
    float fitness = 1.0f / (1.0f + mse);
    agent.SetFitness(fitness);
    return fitness;
}

}  // namespace gen
