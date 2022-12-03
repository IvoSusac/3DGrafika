#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>
#include "geometry.h"
#include "ray.h"
#include "objects.h"
#include "light.h"

using namespace std;

typedef vector<Vec3f> Image;
typedef vector<Object*> Objects;
typedef vector<Light*> Lights;

// funkcija koja ispisuje sliku u .ppm file
void save_image(const Image &image, const int width, const int height, const string path)
{
    ofstream ofs;
    ofs.open(path, ofstream::binary);
    
    // format ppm
    ofs << "P6\n" << width << " " << height << "\n255\n";
    
    // ispis pixela
    for (int i = 0; i < width * height; ++i)
    {
        ofs << (char)(255 * min(max(image[i][0], 0.f), 1.f));
        ofs << (char)(255 * min(max(image[i][1], 0.f), 1.f));
        ofs << (char)(255 * min(max(image[i][2], 0.f), 1.f));
    }
    
    // zatvori file
    ofs.close(); 
}

// funkcija koja provjerava sijece li zraka jedan od objekata
bool scene_intersect(const Ray &ray, const Objects &objs, Material &hit_material, Vec3f &hit_point, Vec3f &hit_normal)
{
    float best_dist = numeric_limits<float>::max();
    float dist = numeric_limits<float>::max();
    
    Vec3f normal;
    
    for (auto obj : objs)
    {
        if (obj->ray_intersect(ray, dist, normal) && dist < best_dist)
        {
            best_dist = dist;             // udaljenost do sfere
            hit_material = obj->material; // materijal pogodjene sfere
            hit_normal = normal;          // normala na pogodjeni objekt
            hit_point = ray.origin + ray.direction * dist; // pogodjena tocka
        }
    }
    
    return best_dist < 1000;
}

// funkcija koja vraca boju
Vec3f cast_ray(const Ray &ray, const Objects &objs, const Lights &lights, const float &depth)
{
    int maxDepth = 5;
    Vec3f hit_normal;
    Vec3f hit_point;
    Material hit_material;
    
    if (!scene_intersect(ray, objs, hit_material, hit_point, hit_normal) || depth > maxDepth)
    {
        return Vec3f(0.8, 0.8, 1); // vrati boju pozadine
    }
    else
    {
        float diffuse_light_intensity = 0;
        float specular_light_intensity = 0;
        
        for (auto light : lights) 
        {           
            Vec3f light_dir = (light->position - hit_point).normalize();
            float light_dist = (light->position - hit_point).norm();
            
            // SJENE
            // ideja: - rekurzivno pozovi scene_intersect od objekta do svijetla
            //        - ako se nesto nalazi izmedju svjetla i objekta, 
            //          tada to svijetlo ignoriramo
            Material shadow_hit_material;
            Vec3f shadow_hit_normal;
            Vec3f shadow_hit_point;
            
            // zbog gresaka u zaokrizivanju moze se dogoditi da zraka zapocne
            // unutar samog objekta. Da to izbjegnemu, origin zrake  za malo
            // pomicemo u smjeru zrake
            Vec3f shadow_origin;
            if (light_dir * hit_normal < 0) // skalarni produkt je manji od 0 ako su suprotne orijentacije
            {                               
                shadow_origin = hit_point - hit_normal * 0.001;
            }
            else
            {
                shadow_origin = hit_point + hit_normal * 0.001;
            }
            Ray shadow_ray(shadow_origin, light_dir);
            
            // provjeri hoce li zraka shadow_ray presijecatiobjekt
            if (scene_intersect(shadow_ray, objs, shadow_hit_material, shadow_hit_point, shadow_hit_normal))
            {
                // zraka sijece neki objekt 
                // trebamo jos provjeriti zaklanja li taj objekt svjetlo
                // tj. nalazi li se izmedju hit_point i light->position
                float dist = (shadow_hit_point - hit_point).norm();
                if (dist < light_dist)
                {
                    // objekt zaklanja svijetlo, preskacemo ovu iteraciju
                    continue;
                }
            }
            
            // I / r^2
            float dist_factor = light->intensity / (light_dist * light_dist);
            
            // difuzno sjenacanje (Lambertov model)
            diffuse_light_intensity +=  hit_material.diffuse_coef * dist_factor * max(0.f, hit_normal * light_dir);

            // Blinn - Phong sjencanje
            Vec3f view_dir = (ray.origin - hit_point).normalize();

            Vec3f half_vector = (view_dir + light_dir).normalize();

            specular_light_intensity += hit_material.specular_coef * (dist_factor) * pow(max(0.f, hit_normal * half_vector), hit_material.phong_exp);
        }
        
        Vec3f reflected_ray = ray.direction - hit_normal * (2 * (ray.direction * hit_normal));

        float cosi = hit_normal * ray.direction;

        Vec3f refracted_ray = (ray.direction * hit_material.refract_coef - hit_normal * (-cosi + hit_material.refract_coef * cosi));

        Vec3f diffuse_color = hit_material.diffuse_color * diffuse_light_intensity;

        Vec3f hit_color = diffuse_color + Vec3f(1, 1, 1) * specular_light_intensity;

        return hit_color = (hit_color + cast_ray(Ray(hit_point + hit_normal * 0.001, reflected_ray), objs, lights, depth + 1) * hit_material.reflex_coef) * hit_material.opacity + cast_ray(Ray(hit_point + hit_normal * 0.001, refracted_ray), objs, lights, depth + 1) * (1 - hit_material.opacity);
    }
}

// funkcija koja napravi zraku iz točke origin
// koja prolazi kroz pixel (i, j) na slici
// (formula s predavanja 3)
Ray ray_to_pixel(Vec3f origin, int i, int j, int width, int height)
{
    Ray ray = Ray();
    ray.origin = origin;
    
    float fov = 1.855; // 106.26° u radijanima
    float tg = tan(fov / 2.);
    
    float x =  (-1 + 2 * (i + 0.5) / (float)width) * tg;
    float y = -(-1 + 2 * (j + 0.5) / (float)height);
    float z = -1;
    
    ray.direction = Vec3f(x, y, z).normalize();
    return ray;
}

void draw_image(Objects objs, Lights lights)
{
    // dimenzije slike
    const int width = 1024;
    const int height = 768;
    
    Image img(width * height);
    
    // ishodište zrake
    Vec3f origin = Vec3f(0, 0, 0);
    
    // crtanje slike, pixel po pixel
    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            Ray ray = ray_to_pixel(origin, i, j, width, height);
            Vec3f color = cast_ray(ray, objs, lights, 0);
            img[i + j * width] = color;
        }
    }
    
    // snimi sliku na disk
    save_image(img, width, height, "./render.ppm");
}

int main()
{
    // definiraj materijale
    Material red(Vec3f(1, 0, 0), 1);
    red.specular_coef = 1;
    red.phong_exp = 50;
    
    Material green(Vec3f(0, 0.5, 0), 0.5);
    green.specular_coef = 1;
    green.phong_exp = 1000;
    
    Material blue(Vec3f(0, 0, 1), 0.5);
    blue.specular_coef = 1;
    blue.phong_exp = 200;
    
    Material grey(Vec3f(0.5, 0.5, 0.5), 1);

    
    // definiraj objekte u sceni
    Cuboid plane(Vec3f(-30, -5, -30), Vec3f(30, -4.5, 9), grey);
    Sphere s1(Vec3f(-1.5, -3, -12), 1.5, green);
    Sphere s2(Vec3f(3, -4, -11), 0.5, red);
    Cuboid c1(Vec3f(7, 0,  -15), Vec3f(10, -7,  -10), green);
    Cuboid c2(Vec3f(-7, 0,  -15), Vec3f(-10, -7,  -10), blue);
    Objects objs = { &plane, &s1, &s2, &c1, &c2 };
    
    // definiraj svjetla
    Light l1 = Light(Vec3f(-20, 20, 20), 3000);
    Light l2 = Light(Vec3f(20, 30, 20), 4000);
    Lights lights = { &l1, &l2 };
    
    draw_image(objs, lights);
    return 0;
}