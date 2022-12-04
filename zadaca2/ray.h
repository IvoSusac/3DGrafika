#pragma once
#include "geometry.h"

class Ray 
{ 
  public: 
    Ray() : origin(Vec3f(0, 0, 0)), direction(Vec3f(0, 0, -1)) {}
    
    Ray(const Vec3f &e, const Vec3f &d) : origin(e), direction(d) {} 
    
    Vec3f origin; 
    Vec3f direction; 
}; 

//