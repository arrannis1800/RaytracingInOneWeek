#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <random>
#include <cstdio>
#include <limits>
#include <memory>
#include <chrono>
#include <vector>


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
    int minutes = seconds / 60;
    seconds -= minutes * 60;
    printf("elapsed time: %d:%d:%f", minutes ,seconds, miliseconds);
}



#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

std::vector<color> rendered_image;

inline void configure_render_image(int width, int height)
{
    rendered_image.resize(width * height);
}

inline void write_file(int width, int height)
{
    printf("Start writing\n");
    FILE* pFile = fopen("output.ppm", "w");

    if (!pFile)
    {
        printf("ERROR: file wasn't opened");
        return;
    }

    fprintf(pFile, "P3\n%d %d\n255\n", width, height);

    for (auto& pixel : rendered_image)
    {
        write_color(pFile, pixel);
    }

    fclose(pFile);
    printf("Writing done\n");
}

#endif // !UTILITY_H