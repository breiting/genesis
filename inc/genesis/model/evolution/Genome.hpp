#pragma once
#include <cstddef>
#include <string>
#include <vector>

namespace gen {

/**
 * @brief Represents the genetic encoding of a neural agent.
 *
 * GENOTYPE
 *
 * Conceptual ASCII layout:
 *
 *   +---------------------------------------------------+
 *   |  Weights + Biases + (future gene extensions ...)  |
 *   +---------------------------------------------------+
 *   ^                     DNA (float sequence)          ^
 *
 * Each Genome contains a contiguous vector<float> of all
 * numerical parameters used to build a neural network.
 */
class Genome {
   public:
    explicit Genome(size_t geneCount = 0);

    // --- DNA Access ---
    void SetDNA(const std::vector<float>& dna);
    const std::vector<float>& GetDNA() const;
    size_t GetGeneCount() const;

    // --- Fitness ---
    void SetFitness(float f);
    float GetFitness() const;

    // --- Genetic Operators ---
    void Mutate(float rate = 0.05f, float magnitude = 0.3f);
    static Genome Crossover(const Genome& a, const Genome& b);

    // --- Persistence ---
    bool SaveBinary(const std::string& path) const;
    bool LoadBinary(const std::string& path);

   private:
    std::vector<float> m_DNA;  ///< Flat genetic sequence
    float m_Fitness{0.0f};
};

}  // namespace gen
