#include <genesis/model/evolution/FitnessTracker.hpp>
#include <numeric>

namespace gen {

void FitnessTracker::AddGeneration(const std::vector<float>& fitness) {
    if (fitness.empty()) return;

    float avg = std::accumulate(fitness.begin(), fitness.end(), 0.0f) / fitness.size();
    float best = *std::max_element(fitness.begin(), fitness.end());

    m_Avg.push_back(avg);
    m_Best.push_back(best);
}

}  // namespace gen
