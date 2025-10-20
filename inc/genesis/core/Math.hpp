#pragma once
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <random>
#include <vector>

namespace gen {

// ============================================================
// === Constants
// ============================================================

constexpr float PI = 3.14159265358979323846f;
constexpr float DEG2RAD = PI / 180.0f;
constexpr float RAD2DEG = 180.0f / PI;

// ============================================================
// === Geometry Structures
// ============================================================

struct Line {
    glm::vec2 a;
    glm::vec2 b;
};

struct Polygon {
    std::vector<glm::vec2> vertices;
};

// ============================================================
// === Random Number Utilities
// ============================================================

// Central RNG-Instance
inline std::mt19937& GlobalRNG() {
    static thread_local std::mt19937 gen{std::random_device{}()};
    return gen;
}

inline float RandFloat(float min = 0.0f, float max = 1.0f) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(GlobalRNG());
}

inline int RandInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(GlobalRNG());
}

inline bool RandBool(float trueChance = 0.5f) {
    std::bernoulli_distribution dist(trueChance);
    return dist(GlobalRNG());
}

// Random pick in a vector
template <typename T>
inline const T& RandChoice(const std::vector<T>& v) {
    return v[RandInt(0, static_cast<int>(v.size()) - 1)];
}

// ============================================================
// === Math / Utility Functions
// ============================================================

inline float Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

inline float Clamp(float x, float minVal, float maxVal) {
    return std::max(minVal, std::min(x, maxVal));
}

inline float SmoothStep(float edge0, float edge1, float x) {
    x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return x * x * (3 - 2 * x);
}

inline float Distance(const glm::vec2& a, const glm::vec2& b) {
    return glm::length(b - a);
}

// Safe normalization
inline glm::vec2 SafeNormalize(const glm::vec2& v) {
    float len = glm::length(v);
    return (len > 1e-6f) ? v / len : glm::vec2(0.0f);
}

// ============================================================
// === Activation Functions
// ============================================================

inline float Sigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

inline float SigmoidDerivative(float x) {
    const float s = Sigmoid(x);
    return s * (1.0f - s);
}

inline float Tanh(float x) {
    return std::tanh(x);
}

inline float TanhDerivative(float x) {
    const float t = std::tanh(x);
    return 1.0f - t * t;
}

inline float ReLU(float x) {
    return x > 0.0f ? x : 0.0f;
}

inline float ReLUDerivative(float x) {
    return x > 0.0f ? 1.0f : 0.0f;
}

inline float LeakyReLU(float x, float alpha = 0.01f) {
    return x > 0.0f ? x : alpha * x;
}

inline float LeakyReLUDerivative(float x, float alpha = 0.01f) {
    return x > 0.0f ? 1.0f : alpha;
}

inline float Softsign(float x) {
    return x / (1.0f + std::fabs(x));
}

inline float SoftsignDerivative(float x) {
    const float denom = 1.0f + std::fabs(x);
    return 1.0f / (denom * denom);
}

// ============================================================
// === Random Vector Generation
// ============================================================

inline glm::vec2 RandUnitVec2() {
    float angle = RandFloat(0.0f, 2.0f * PI);
    return glm::vec2(std::cos(angle), std::sin(angle));
}

inline glm::vec2 RandInCircle(float radius = 1.0f) {
    float angle = RandFloat(0.0f, 2.0f * PI);
    float r = std::sqrt(RandFloat(0.0f, 1.0f)) * radius;
    return glm::vec2(std::cos(angle) * r, std::sin(angle) * r);
}

// Shuffle vector
template <typename T>
inline void Shuffle(std::vector<T>& v) {
    std::shuffle(v.begin(), v.end(), GlobalRNG());
}

}  // namespace gen
