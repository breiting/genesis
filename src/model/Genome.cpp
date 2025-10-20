#include <cassert>
#include <fstream>
#include <genesis/core/Math.hpp>
#include <genesis/model/Genome.hpp>

namespace gen {

Genome::Genome(size_t geneCount) : m_DNA(geneCount, 0.0f) {
}

// ============================================================
// === Getters / Setters
// ============================================================

void Genome::SetDNA(const std::vector<float>& dna) {
    m_DNA = dna;
}

const std::vector<float>& Genome::GetDNA() const {
    return m_DNA;
}

size_t Genome::GetGeneCount() const {
    return m_DNA.size();
}

void Genome::SetFitness(float f) {
    m_Fitness = f;
}

float Genome::GetFitness() const {
    return m_Fitness;
}

// ============================================================
// === Genetic Operators
// ============================================================

void Genome::Mutate(float rate, float magnitude) {
    for (auto& g : m_DNA) {
        if (RandFloat(0.0f, 1.0f) < rate) {
            g += RandFloat(-magnitude, magnitude);
        }
    }
}

Genome Genome::Crossover(const Genome& a, const Genome& b) {
    assert(a.m_DNA.size() == b.m_DNA.size());
    Genome child(a.m_DNA.size());

    for (size_t i = 0; i < a.m_DNA.size(); ++i) {
        // Uniform crossover: each gene randomly taken from parent A or B
        child.m_DNA[i] = (RandFloat(0.0f, 1.0f) < 0.5f) ? a.m_DNA[i] : b.m_DNA[i];
    }

    return child;
}

// ============================================================
// === Binary Serialization
// Format: | uint64_t count | float[count] weights+biases | float fitness |
// ============================================================

bool Genome::SaveBinary(const std::string& path) const {
    std::ofstream file(path, std::ios::binary);
    if (!file) return false;

    const uint64_t count = static_cast<uint64_t>(m_DNA.size());
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));
    file.write(reinterpret_cast<const char*>(m_DNA.data()), static_cast<std::streamsize>(count * sizeof(float)));
    file.write(reinterpret_cast<const char*>(&m_Fitness), sizeof(m_Fitness));
    return true;
}

bool Genome::LoadBinary(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;

    uint64_t count = 0;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));

    m_DNA.resize(count);
    file.read(reinterpret_cast<char*>(m_DNA.data()), static_cast<std::streamsize>(count * sizeof(float)));
    file.read(reinterpret_cast<char*>(&m_Fitness), sizeof(m_Fitness));

    return true;
}

}  // namespace gen
