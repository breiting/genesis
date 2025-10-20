#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

namespace gen {

/** Returns the time in seconds
 */
inline double Now() {
    using clock = std::chrono::steady_clock;
    return std::chrono::duration<double>(clock::now().time_since_epoch()).count();
}

class TimeMeasureGuard {
   public:
    TimeMeasureGuard(const std::string& label) : m_Label(label), m_Start(std::chrono::high_resolution_clock::now()) {
    }

    ~TimeMeasureGuard() {
        using namespace std::chrono;
        auto end = high_resolution_clock::now();
        double ms = duration<double, std::milli>(end - m_Start).count();

        auto& stat = s_Stats[m_Label];
        stat.accumulated += ms;
        stat.count++;

        double elapsed = duration<double>(end - stat.lastReport).count();
        if (elapsed >= 1.0) {
            double avg = stat.accumulated / stat.count;
            std::cout << m_Label << " avg: " << avg << " ms (over " << stat.count << " frames)" << std::endl;

            stat.accumulated = 0.0;
            stat.count = 0;
            stat.lastReport = end;
        }
    }

   private:
    std::string m_Label;
    std::chrono::high_resolution_clock::time_point m_Start;

    struct Stat {
        double accumulated = 0.0;
        int count = 0;
        std::chrono::high_resolution_clock::time_point lastReport = std::chrono::high_resolution_clock::now();
    };

    static inline std::unordered_map<std::string, Stat> s_Stats;
};
}  // namespace gen
