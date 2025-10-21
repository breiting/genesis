#include <cassert>
#include <genesis/core/Math.hpp>
#include <genesis/model/Population.hpp>

namespace gen {

Population::Population(size_t size, size_t geneCount) {
    m_Genomes.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        m_Genomes.emplace_back(geneCount);
    }
}

std::vector<Genome>& Population::GetGenomes() {
    return m_Genomes;
}

const Genome& Population::GetBest() const {
    assert(!m_Genomes.empty());
    return *std::max_element(m_Genomes.begin(), m_Genomes.end(),
                             [](const Genome& a, const Genome& b) { return a.GetFitness() < b.GetFitness(); });
}

// ============================================================
// === Evolution Mechanics
// ============================================================

void Population::SortByFitness() {
    std::sort(m_Genomes.begin(), m_Genomes.end(), [](const Genome& a, const Genome& b) {
        return a.GetFitness() > b.GetFitness();  // descending
    });
}

float Population::GetAverageFitness() const {
    float sum = 0.0f;
    for (const auto& g : m_Genomes) sum += g.GetFitness();
    return sum / static_cast<float>(m_Genomes.size());
}

void Population::Evolve(float mutationRate, float mutationMag, float elitism) {
    assert(!m_Genomes.empty());
    SortByFitness();

    const size_t eliteCount = static_cast<size_t>(elitism * m_Genomes.size());
    const size_t popSize = m_Genomes.size();

    std::vector<Genome> newPop;
    newPop.reserve(popSize);

    // 1️⃣ Copy elites
    for (size_t i = 0; i < eliteCount; ++i) {
        newPop.push_back(m_Genomes[i]);
    }

    // 2️⃣ Crossover & mutation for rest
    while (newPop.size() < popSize) {
        const Genome& parentA = m_Genomes[RandInt(0, eliteCount > 0 ? eliteCount - 1 : 0)];
        const Genome& parentB = m_Genomes[RandInt(0, eliteCount > 0 ? eliteCount - 1 : 0)];

        Genome child = Genome::Crossover(parentA, parentB);
        child.Mutate(mutationRate, mutationMag);
        newPop.push_back(std::move(child));
    }

    m_Genomes = std::move(newPop);
}

}  // namespace gen
