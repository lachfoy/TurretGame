#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <random>

namespace Random
{
inline std::mt19937 gGenerator{std::random_device{}()};

inline void Init(std::uint32_t seed) { gGenerator.seed(seed); }

inline int Int(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gGenerator);
}

inline float Float(float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(gGenerator);
}

inline bool Chance(float probability)
{
    std::bernoulli_distribution distribution(probability);
    return distribution(gGenerator);
}

inline float Angle() { return Float(0.0f, glm::two_pi<float>()); }

inline glm::vec2 Direction()
{
    const float angle = Angle();
    return {std::cos(angle), std::sin(angle)};
}
} // namespace Random