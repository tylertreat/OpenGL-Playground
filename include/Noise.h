#ifndef NOISE_H
#define NOISE_H

#include <Angel.h>

class Noise
{
public:
    static float Noise1D(float x);

    static float Noise2D(float x, float y);
    inline static float Noise2D(const vec2& xy)
    {
        return Noise2D(xy.x, xy.y);
    }

    static float Noise3D(float x, float y, float z);
    inline static float Noise3D(const vec3& xyz)
    {
        return Noise3D(xyz.x, xyz.y, xyz.z);
    }

    static float LoopNoise1D(float x, float period);

    static float LoopNoise2D(float x, float y, float yPeriod);
    inline static float LoopNoise2D(const vec2& xy, float yPeriod)
    {
        return LoopNoise2D(xy.x, xy.y, yPeriod);
    }

    static float LoopNoise3D(float x, float y, float z, float zPeriod);
    inline static float LoopNoise3D(const vec3& xyz, float zPeriod)
    {
        return LoopNoise3D(xyz.x, xyz.y, xyz.z, zPeriod);
    }

    static float TileNoise2D(float x, float y, float width, float height);
    inline static float TileNoise2D(const vec2& xy, float width, float height)
    {
        return TileNoise2D(xy.x, xy.y, width, height);
    }

    static float TileNoise3D(
        float x, float y, float z, 
        float width, float height, float depth);
    inline static float TileNoise3D(
        const vec3& xyz, float width, float height, float depth)
    {
        return TileNoise3D(xyz.x, xyz.y, xyz.z, width, height, depth);
    }

private:
    Noise();                        //!< No default constructor
    Noise(const Noise&);            //!< No copy constructor
    Noise& operator=(const Noise&); //!< No assignment operator
    ~Noise();                       //!< No destructor
};

#endif
