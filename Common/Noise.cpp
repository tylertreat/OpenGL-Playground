#include "Noise.h"

// Prototypes from Perlin's original implementation
extern "C"
{
    float noise1(float arg);
    float noise2(float vec[2]);
    float noise3(float vec[3]);
};

/*
 * 1D noise
 */
float Noise::Noise1D(float x)
{
    return noise1(x);
}

/*
 * 2D noise
 */
float Noise::Noise2D(float x, float y)
{
    float vec[2] = { x, y };
    return noise2(vec);
}

/*
 * 3D noise
 */
float Noise::Noise3D(float x, float y, float z)
{
    float vec[3] = { x, y, z };
    return noise3(vec);
}

/*
 * Looping 1D noise
 */
float Noise::LoopNoise1D(float x, float period)
{
    return ((period - x) * Noise1D(x) + x * Noise1D(period - x)) / period;
}

/*
 * Looping 2D noise
 */
float Noise::LoopNoise2D(float x, float y, float yPeriod)
{
    return ((yPeriod - y) * Noise2D(x,y) + 
        y * Noise2D(x, yPeriod - y)) / yPeriod;
}

/*
 * Looping 3D noise
 */
float Noise::LoopNoise3D(float x, float y, float z, float zPeriod)
{
    return ((zPeriod - z) * Noise3D(x,y,z) + 
        z * Noise3D(x, y, zPeriod - z)) / zPeriod;
}

/*
 * Tileable 2D noise
 */
float Noise::TileNoise2D(float x, float y, float width, float height)
{
    float invX = width  - x;
    float invY = height - y;
    return (
        Noise2D(x,    y)    * invX * invY +
        Noise2D(invX, y)    * x    * invY +
        Noise2D(invX, invY) * x    * y +
        Noise2D(x,    invY) * invX * y
        ) / (width * height);
}

/*
 * Tileable 3D noise
 */
float Noise::TileNoise3D(
    float x, float y, float z, 
    float width, float height, float depth)
{
    float invX = width  - x;
    float invY = height - y;
    float invZ = depth  - z;
    return (
        Noise3D(x,    y,    z)    * invX * invY * invZ +
        Noise3D(invX, y,    z)    * x    * invY * invZ +
        Noise3D(x,    invY, z)    * invX * y    * invZ +
        Noise3D(x,    y,    invZ) * invX * invY * z    +
        Noise3D(invX, invY, z)    * x    * y    * invZ +
        Noise3D(invX, y,    invZ) * x    * invY * z    +
        Noise3D(x,    invY, invZ) * invX * y    * z    +
        Noise3D(invX, invY, invZ) * x    * y    * z
        ) / (width * height * depth);
}
