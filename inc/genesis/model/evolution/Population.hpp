#pragma once
#include <vector>

#include "Genome.hpp"

namespace gen {

/**
 * @brief Represents a population of genomes that evolve over generations.
 *
 * Each generation:
 *  1. Sort by fitness (descending)
 *  2. Copy top N% as elites
 *  3. Fill rest by crossover + mutation
 *
 * The Population itself does not evaluate fitness — that is handled externally.
 */
class Population {
   public:
    Population(size_t size, size_t geneCount);

    // --- Access ---
    std::vector<Genome>& GetGenomes();
    const Genome& GetBest() const;

    // --- Evolution steps ---
    void SortByFitness();
    void Evolve(float mutationRate, float mutationMag, float elitism = 0.1f);
    float GetAverageFitness() const;

    size_t Size() const {
        return m_Genomes.size();
    }

   private:
    std::vector<Genome> m_Genomes;
};
}  // namespace gen
