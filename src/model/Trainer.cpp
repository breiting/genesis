#include <algorithm>
#include <genesis/model/Trainer.hpp>
#include <iostream>

namespace gen {

Trainer::Trainer(std::vector<std::unique_ptr<Agent>>& agents, std::unique_ptr<IFitnessEvaluator> evaluator,
                 const std::string& capabilityName)
    : m_Agents(agents), m_Evaluator(std::move(evaluator)), m_CapabilityName(capabilityName) {
}

void Trainer::RunGeneration(float dt, size_t steps, float mutationRate, float mutationMag, float elitism) {
    // 1. Simulate
    for (size_t step = 0; step < steps; ++step)
        for (auto& a : m_Agents) a->Update(dt);

    // 2. Evaluate fitness
    m_AvgFitness = 0.0f;
    m_BestFitness = 0.0f;

    for (auto& a : m_Agents) {
        float f = m_Evaluator->Evaluate(*a);
        m_AvgFitness += f;
        m_BestFitness = std::max(m_BestFitness, f);
    }
    m_AvgFitness /= m_Agents.size();

    // 3. Evolve
    std::sort(m_Agents.begin(), m_Agents.end(),
              [](const auto& a, const auto& b) { return a->GetFitness() > b->GetFitness(); });

    size_t eliteCount = static_cast<size_t>(m_Agents.size() * elitism);
    size_t popSize = m_Agents.size();

    // ---- 1. Elite kopieren ----
    std::vector<std::vector<float>> eliteDNA;
    eliteDNA.reserve(eliteCount);

    for (size_t i = 0; i < eliteCount; ++i) {
        auto* cap = m_Agents[i]->GetCapability(m_CapabilityName);
        if (!cap) continue;
        eliteDNA.push_back(cap->GetBrain().GetWeightsAndBiases());
    }

    // ---- 2. Rest erzeugen durch Mutation der Elite ----
    for (size_t i = eliteCount; i < popSize; ++i) {
        auto* cap = m_Agents[i]->GetCapability(m_CapabilityName);
        if (!cap) continue;

        // Wähle zufällig ein Elite-Gen
        const auto& parentDNA = eliteDNA[RandInt(0, (int)eliteDNA.size() - 1)];

        // Kopiere & mutiere
        std::vector<float> childDNA = parentDNA;
        for (auto& w : childDNA)
            if (RandFloat(0, 1) < mutationRate) w += RandFloat(-mutationMag, mutationMag);

        cap->GetBrain().SetWeightsAndBiases(childDNA);
    }

    ++m_GenerationCount;
    std::cout << "Generation finished: avg=" << m_AvgFitness << " best=" << m_BestFitness << std::endl;
}

const Genome& Trainer::GetBestGenome() const {
    return m_Agents[0]->GetCapability(m_CapabilityName)->GetGenome();
}

}  // namespace gen
