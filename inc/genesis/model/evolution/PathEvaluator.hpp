#pragma once
#include <genesis/model/environment/PathEnvironment.hpp>
#include <genesis/model/evolution/Genome.hpp>
#include <genesis/model/evolution/NeuralNetwork.hpp>
#include <memory>

namespace gen {

/**
 * @brief Evaluates genomes by running them in the PathEnvironment.
 */
class PathEvaluator {
   public:
    PathEvaluator(std::shared_ptr<NeuralNetwork> net, PathEnvironment& env) : m_NN(std::move(net)), m_Env(env) {
    }

    float Evaluate(Genome& g) {
        m_NN->SetWeightsAndBiases(g.GetDNA());
        return m_Env.RunEpisode(*m_NN);
    }

   private:
    std::shared_ptr<NeuralNetwork> m_NN;
    PathEnvironment& m_Env;
};

}  // namespace gen
