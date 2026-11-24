#include "Math/Random.hpp"

#include <chrono>
#include <random>

namespace Math {

namespace {
    std::mt19937& engine()
    {
        // Seeded once, from two sources. (std::random_device alone is
        // unreliable on some older MinGW versions, so the clock is mixed in.)
        static std::mt19937 e([] {
            std::random_device rd;
            auto t = static_cast<unsigned int>(
                std::chrono::high_resolution_clock::now().time_since_epoch().count());
            std::seed_seq seq{ rd(), t };
            return std::mt19937(seq);
        }());
        return e;
    }
}

void Random::setSeed(unsigned int seed)
{
    engine().seed(seed);
}

int Random::nextInt(int min, int max)
{
    if (min > max) { int t = min; min = max; max = t; }
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine());
}

float Random::nextFloat()
{
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(engine());
}

float Random::nextFloat(float min, float max)
{
    if (min >= max) return min;
    std::uniform_real_distribution<float> dist(min, max);
    return dist(engine());
}

bool Random::nextBool()
{
    return nextInt(0, 1) == 1;
}

} // namespace Math