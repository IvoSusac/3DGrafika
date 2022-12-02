#pragma once
#include "geometry.h"

struct Light
{
    Vec3f position;
    float intensity;
    
    Light(const Vec3f &position, const float &intensity) : position(position), intensity(intensity) {}
};