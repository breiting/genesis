#pragma once

#include <vector>

/**
 * \brief An interface for calculating the fitness for a given DNA
 */
class IFitnessEvaluator {
   public:
    virtual float Evaluate(const std::vector<float>& dna) = 0;
    virtual ~IFitnessEvaluator() = default;
};
