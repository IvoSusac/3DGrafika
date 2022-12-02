#pragma once
#include <cmath>
#include "geometry.h"
#include "material.h"
#include <iostream>
#include <limits>
using namespace std;

struct Object
{
    Material material;
    
    virtual bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const = 0;
};

struct Sphere : Object
{
    Vec3f c; // centar
    float r; // radius
    
    Sphere(const Vec3f &c, const float &r, const Material &mat) : c(c), r(r)
    {
        Object::material = mat;
    }
    
    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const
    {
        float d2 = ray.direction * ray.direction;
        Vec3f e_minus_c = ray.origin - c;
        
        float disc = pow(ray.direction * e_minus_c, 2) - d2 * (e_minus_c * e_minus_c - r * r);
        
        bool ray_inside_sphere = e_minus_c * e_minus_c <= r * r;
        
        if (disc < 0)
        {
            return false;
        }
        else
        {
            if (ray_inside_sphere)
            {
                t = (-ray.direction * e_minus_c + sqrt(disc))/d2;
            }
            else
            {
                t = (-ray.direction * e_minus_c - sqrt(disc))/d2;                
            }
            
            Vec3f normal_origin = ray.origin + ray.direction * t;
            normal = (normal_origin - c).normalize();
            
            return t > 0;
        }
    }
};

struct Cuboid : Object {

    Vec3f c1; 
    Vec3f c2; 

    Cuboid(const Vec3f &c1, const Vec3f &c2, const Material &mat) : c1(c1), c2(c2) { Object::material = mat; }

    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const {
        float tMin = numeric_limits<float>::min();
        float tMax = numeric_limits<float>::max();

        float minX = min(c1.x, c2.x);
        float maxX = max(c1.x, c2.x);
        float minY = min(c1.y, c2.y);
        float maxY = max(c1.y, c2.y);
        float minZ = min(c1.z, c2.z);
        float maxZ = max(c1.z, c2.z);

        if (ray.direction.x == 0) {
            if (ray.origin.x < minX || ray.origin.x > maxX)
                return false;
        }
        else {
            float t1 = (minX - ray.origin.x) / ray.direction.x;
            float t2 = (maxX - ray.origin.x) / ray.direction.x;

            if (t1 > t2) {
                swap(t1, t2);
            }

            tMin = max(tMin, t1);
            tMax = min(tMax, t2);

            if (tMin > tMax || tMax < 0)
                return false;
        }

        t = tMin;

        if (ray.direction.y == 0) {
            if (ray.origin.y < minY || ray.origin.y > maxY)
                return false;
        }
        else {
            float t1 = (minY - ray.origin.y) / ray.direction.y;
            float t2 = (maxY - ray.origin.y) / ray.direction.y;

            if (t1 > t2) {
                swap(t1, t2);
            }

            tMin = max(tMin, t1);
            tMax = min(tMax, t2);

            if (tMin > tMax || tMax < 0)
                return false;
        }

        t = tMin;

        if (ray.direction.z == 0) {
            if (ray.origin.z < minZ || ray.origin.z > maxZ)
                return false;
        }
        else {
            float t1 = (minZ - ray.origin.z) / ray.direction.z;
            float t2 = (maxZ - ray.origin.z) / ray.direction.z;

            if (t1 > t2) {
                swap(t1, t2);
            }

            tMin = max(tMin, t1);
            tMax = min(tMax, t2);

            if (tMin > tMax || tMax < 0)
                return false;
        }

        t = tMin;

        if (abs(ray.origin.x - c1.x) < 0.01)
            normal = Vec3f(-1, 0, 0);
        else if (abs(ray.origin.x - c2.x) < 0.01)
            normal = Vec3f(1, 0, 0);
        else if (abs(ray.origin.y - c1.y) < 0.01)
            normal = Vec3f(0, -1, 0);
        else if (abs(ray.origin.y - c2.y) < 0.01)
            normal = Vec3f(0, 1, 0);
        else if (abs(ray.origin.z - c1.z) < 0.01)
            normal = Vec3f(0, 0, -1);
        else if (abs(ray.origin.z - c2.z) < 0.01)
            normal = Vec3f(0, 0, 1);

        return true;
    }

};