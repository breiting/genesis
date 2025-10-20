#pragma once
#include <vector>

namespace gen {

/**
 * @brief Tracks fitness evolution across generations for visualization.
 */
class FitnessTracker {
   public:
    void AddGeneration(const std::vector<float>& fitness);

    const std::vector<float>& GetAverageHistory() const {
        return m_Avg;
    }
    const std::vector<float>& GetBestHistory() const {
        return m_Best;
    }

    float GetLastAverage() const {
        return m_Avg.empty() ? 0.0f : m_Avg.back();
    }
    float GetLastBest() const {
        return m_Best.empty() ? 0.0f : m_Best.back();
    }

   private:
    std::vector<float> m_Avg;
    std::vector<float> m_Best;
};

}  // namespace gen
