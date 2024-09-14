#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <random>
#include <cstdio>
#include <limits>
#include <memory>
#include <chrono>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
static std::chrono::steady_clock::time_point time_start;
static std::chrono::steady_clock::time_point time_end;


inline double degrees_to_radians(double degrees) 
{
    return degrees * pi / 180.0;
}

inline double random_double() 
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) 
{
    return min + (max - min) * random_double();
}

inline void start_timer()
{
    time_start = std::chrono::high_resolution_clock::now();
}

inline void stop_timer()
{
    time_end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start).count();
    int seconds = duration / 1000 / 1000;
    float miliseconds = (duration - seconds * 1000 * 1000) / 1000;
    printf("elapsed time: %d:%f", seconds, miliseconds);
}

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif // !UTILITY_H