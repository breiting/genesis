#pragma once
#include <cstddef>
#include <genesis/core/Math.hpp>
#include <vector>

namespace gen {

enum class ActivationType {
    Sigmoid,    // Output is probability or intensity (output)
    Tanh,       // For positive and negative signals (e.g. direction control), (hidden/input)
    ReLU,       // For deeper networks with fast convergence (hidden)
    LeakyReLU,  // for random initialization (hidden)
    Softsign,   // smooth transitions (e.g. motor control), (output)
    Linear,     // Linear return
};

inline float Activate(ActivationType type, float x) {
    switch (type) {
        case ActivationType::Sigmoid:
            return Sigmoid(x);
        case ActivationType::Tanh:
            return Tanh(x);
        case ActivationType::ReLU:
            return ReLU(x);
        case ActivationType::LeakyReLU:
            return LeakyReLU(x);
        case ActivationType::Softsign:
            return Softsign(x);
        case ActivationType::Linear:
            return x;
    }
    return x;
}

/**
 * @brief Feed-forward neural network
 *
 * PHENOTYPE
 *
 * Works with weights and biases. Every layer can have its own activation function
 */
class NeuralNetwork {
   public:
    explicit NeuralNetwork(const std::vector<int>& layout);

    /// Feed-forward inference
    std::vector<float> Forward(const std::vector<float>& input) const;

    /// Weight and bias management
    void SetWeightsAndBiases(const std::vector<float>& weights);
    std::vector<float> GetWeightsAndBiases() const;

    /// Activation functions
    void SetActivation(size_t layerIndex, ActivationType type);

    size_t GetWeightCount() const;
    size_t GetBiasCount() const;
    size_t GetLayerCount() const;

    /// Initialization
    void Randomize(float range = 1.0f);

    /// Export network to Graphviz
    void ExportDot(const std::string& fileName) const;

   private:
    /// Number of neurons in each layer (e.g. {2, 8, 2})
    std::vector<int> m_Layers;

    /// Flattened weights: layer-wise (in -> out)
    std::vector<float> m_Weights;

    /// Flattened biases: one per neuron (except input layer)
    std::vector<float> m_Biases;

    /// Offsets into the m_Weights array
    std::vector<size_t> m_WeightOffsets;

    /// Offsets into the m_Biases array
    std::vector<size_t> m_BiasOffsets;

    /// Per-layer activation function (except input)
    std::vector<ActivationType> m_Activations;

    void ComputeOffsets();
};

}  // namespace gen
