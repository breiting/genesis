#include <algorithm>
#include <cassert>
#include <fstream>
#include <genesis/core/Math.hpp>
#include <genesis/model/NeuralNetwork.hpp>
#include <stdexcept>

namespace gen {

NeuralNetwork::NeuralNetwork(const std::vector<int>& layout) : m_Layers(layout) {
    ComputeOffsets();

    m_Weights.resize(GetWeightCount());
    m_Biases.resize(GetBiasCount());

    // Default: all hidden layers = ReLU, Output = Sigmoid
    m_Activations.resize(layout.size() - 1, ActivationType::ReLU);
    m_Activations.back() = ActivationType::Sigmoid;

    Randomize();
}

void NeuralNetwork::ComputeOffsets() {
    m_WeightOffsets.clear();
    m_BiasOffsets.clear();

    size_t wOffset = 0;
    size_t bOffset = 0;

    for (size_t i = 1; i < m_Layers.size(); ++i) {
        m_WeightOffsets.push_back(wOffset);
        m_BiasOffsets.push_back(bOffset);

        wOffset += static_cast<size_t>(m_Layers[i - 1]) * static_cast<size_t>(m_Layers[i]);
        bOffset += static_cast<size_t>(m_Layers[i]);
    }
}

std::vector<float> NeuralNetwork::Forward(const std::vector<float>& input) const {
    assert(!m_Layers.empty());
    assert(static_cast<int>(input.size()) == m_Layers.front());

    std::vector<float> activations = input;

    for (size_t layer = 1; layer < m_Layers.size(); ++layer) {
        const int inCount = m_Layers[layer - 1];
        const int outCount = m_Layers[layer];
        const auto actType = m_Activations[layer - 1];

        const float* weights = &m_Weights[m_WeightOffsets[layer - 1]];
        const float* biases = &m_Biases[m_BiasOffsets[layer - 1]];

        std::vector<float> next(outCount, 0.0f);

        for (int o = 0; o < outCount; ++o) {
            const float* w = weights + o * inCount;
            float sum = biases[o];  // bias per neuron

            // weighted input sum
            for (int i = 0; i < inCount; ++i) {
                sum += w[i] * activations[i];
            }

            next[o] = Activate(actType, sum);
        }

        activations = std::move(next);
    }

    return activations;
}

void NeuralNetwork::SetActivation(size_t layerIndex, ActivationType type) {
    if (layerIndex < m_Activations.size()) {
        m_Activations[layerIndex] = type;
    }
}

void NeuralNetwork::SetWeightsAndBiases(const std::vector<float>& w) {
    const size_t expected = GetWeightCount() + GetBiasCount();
    if (w.size() != expected) {
        throw std::runtime_error("Length of flattened weights and biases does not match");
    }

    const size_t wCount = GetWeightCount();
    std::copy_n(w.begin(), wCount, m_Weights.begin());
    std::copy_n(w.begin() + wCount, GetBiasCount(), m_Biases.begin());
}

std::vector<float> NeuralNetwork::GetWeightsAndBiases() const {
    std::vector<float> all;
    all.reserve(m_Weights.size() + m_Biases.size());
    all.insert(all.end(), m_Weights.begin(), m_Weights.end());
    all.insert(all.end(), m_Biases.begin(), m_Biases.end());
    return all;
}

size_t NeuralNetwork::GetWeightCount() const {
    size_t total = 0;
    for (size_t i = 1; i < m_Layers.size(); ++i)
        total += static_cast<size_t>(m_Layers[i - 1]) * static_cast<size_t>(m_Layers[i]);
    return total;
}

size_t NeuralNetwork::GetBiasCount() const {
    size_t total = 0;
    for (size_t i = 1; i < m_Layers.size(); ++i) total += static_cast<size_t>(m_Layers[i]);
    return total;
}

// ============================================================
// === Initialization
// ============================================================

void NeuralNetwork::Randomize(float range) {
    for (auto& w : m_Weights) w = RandFloat(-range, range);
    for (auto& b : m_Biases) b = RandFloat(-range, range);
}

// ============================================================
// === Representation
// ============================================================

void NeuralNetwork::ExportDot(const std::string& filename) const {
    std::ofstream dot(filename);
    dot << "digraph NeuralNet {\n";
    dot << "rankdir=LR;\n";
    dot << "node [shape=circle, style=filled, fillcolor=lightgray];\n";

    // Nodes
    for (size_t l = 0; l < m_Layers.size(); ++l)
        for (int n = 0; n < m_Layers[l]; ++n) dot << "L" << l << "N" << n << " [label=\"" << l << ":" << n << "\"];\n";

    // Connections
    size_t wOffset = 0;
    for (size_t l = 1; l < m_Layers.size(); ++l) {
        int inCount = m_Layers[l - 1];
        int outCount = m_Layers[l];

        for (int o = 0; o < outCount; ++o)
            for (int i = 0; i < inCount; ++i) {
                float w = m_Weights[wOffset++];
                dot << "L" << (l - 1) << "N" << i << " -> L" << l << "N" << o << " [label=\"" << w << "\", color=\""
                    << (w > 0 ? "green" : "red") << "\"];\n";
            }
    }

    dot << "}\n";
    dot.close();
}

}  // namespace gen
