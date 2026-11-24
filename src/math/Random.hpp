#pragma once

namespace Math {

class Random {
public:
    static void setSeed(unsigned int seed);   // same seed = same sequence (handy for testing)

    static int   nextInt(int min, int max);        // min and max both included
    static float nextFloat();                      // 0.0 up to (not including) 1.0
    static float nextFloat(float min, float max);  // min up to (not including) max
    static bool  nextBool();                       // true or false, 50/50
};

} // namespace Math