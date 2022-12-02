#pragma once
#include "geometry.h"

struct Material
{  
    Vec3f diffuse_color;
    
    float diffuse_coef = 1;
    float specular_coef = 0;
    float phong_exp = 1;
    float reflex_coef = 0.2;
    float refract_coef = 1;
    float opacity;
    
    Material(const Vec3f &color, const float opacity) : diffuse_color(color), opacity(opacity) {}
    Material() : diffuse_color(Vec3f(0, 0, 0)), opacity(1) {}
};