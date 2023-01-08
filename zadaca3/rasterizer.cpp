#include <iostream>
#include <cmath>
#include <algorithm>
#include "tgaimage.h"
using namespace std;

// dimenzije slike
const int width  = 512;
const int height = 512;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green  = TGAColor(0, 255, 0, 255);

void set_color(int x, int y, TGAImage &image, TGAColor color, bool invert = false)
{
    image.set(x, y, color);
}

float line(float x0, float y0, float x1, float y1, float x, float y)
{
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

void draw_triangle_2d(TGAImage& image, float x0, float y0, float x1, float y1, float x2, float y2, TGAColor color)
{
    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float xmax = max(max(ceil(x0), ceil(x1)), ceil(x2));

    float ymin = min(min(floor(y0), floor(y1)), floor(y2));
    float ymax = max(max(ceil(y0), ceil(y1)), ceil(y2));

    for (float y = ymin; y <= ymax; ++y) {
        for (float x = xmin; x <= xmax; ++x) {
            float alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            float beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            float gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1) {
                set_color(x, y, image, color);
            }
        }
    }
}

void draw_triangle_2d_gouraurd(TGAImage& image, float x0, float y0, TGAColor c0, float x1, float y1, TGAColor c1, float x2, float y2, TGAColor c2)
{
    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float xmax = max(max(ceil(x0), ceil(x1)), ceil(x2));

    float ymin = min(min(floor(y0), floor(y1)), floor(y2));
    float ymax = max(max(ceil(y0), ceil(y1)), ceil(y2));

    for (float y = ymin; y <= ymax; ++y) {
        for (float x = xmin; x <= xmax; ++x) {
            float alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            float beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            float gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1) {
                set_color(x, y, image, TGAColor((alpha * c0.r + beta * c1.r + gamma * c2.r), (alpha * c0.g + beta * c1.g + gamma * c2.g), (alpha * c0.b + beta * c1.b + gamma * c2.b), 255));
            }
        }
    }
}

void draw_triangle(TGAImage& image, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, TGAColor color) {

    x0 /= z0;
    y0 /= z0;
    x1 /= z1;
    y1 /= z1;
    x2 /= z2;
    y2 /= z2;

    x0 = (1 + x0) * 0.5 * width;
    x1 = (1 + x1) * 0.5 * width;
    x2 = (1 + x2) * 0.5 * width;

    y0 = (1 + y0) * 0.5 * height;
    y1 = (1 + y1) * 0.5 * height;
    y2 = (1 + y2) * 0.5 * height;

    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float xmax = max(max(ceil(x0), ceil(x1)), ceil(x2));

    float ymin = min(min(floor(y0), floor(y1)), floor(y2));
    float ymax = max(max(ceil(y0), ceil(y1)), ceil(y2));

    for (float y = ymin; y <= ymax; ++y) {
        for (float x = xmin; x <= xmax; ++x) {
            float alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            float beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            float gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1) {
                set_color(x, y, image, color);
            }
        }
    }
}
void draw_triangle_tex(TGAImage& image, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float u0, float v0, float u1, float v1, float u2, float v2, const char* path)
{
    TGAImage texture;
    texture.read_tga_file(path);

    x0 /= z0;
    y0 /= z0;
    x1 /= z1;
    y1 /= z1;
    x2 /= z2;
    y2 /= z2;

    x0 = (1 + x0) * 0.5 * width;
    x1 = (1 + x1) * 0.5 * width;
    x2 = (1 + x2) * 0.5 * width;

    y0 = (1 + y0) * 0.5 * height;
    y1 = (1 + y1) * 0.5 * height;
    y2 = (1 + y2) * 0.5 * height;

    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float xmax = max(max(ceil(x0), ceil(x1)), ceil(x2));

    float ymin = min(min(floor(y0), floor(y1)), floor(y2));
    float ymax = max(max(ceil(y0), ceil(y1)), ceil(y2));

    for (float y = ymin; y <= ymax; ++y) {
        for (float x = xmin; x <= xmax; ++x) {
            float alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            float beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            float gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1) {
                int s = round(texture.get_width() * (alpha * u0 + beta * u1 + gamma * u2));
                int t = round(texture.get_height() * (alpha * v0 + beta * v1 + gamma * v2));
                set_color(x, y, image, texture.get(s, t));
            }
        }
    }
}

void draw_triangle_tex_corrected(TGAImage& image, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float u0, float v0, float u1, float v1, float u2, float v2, const char* path)
{
    TGAImage texture;
    texture.read_tga_file(path);

    x0 /= z0;
    y0 /= z0;
    x1 /= z1;
    y1 /= z1;
    x2 /= z2;
    y2 /= z2;

    x0 = (1 + x0) * 0.5 * width;
    x1 = (1 + x1) * 0.5 * width;
    x2 = (1 + x2) * 0.5 * width;

    y0 = (1 + y0) * 0.5 * height;
    y1 = (1 + y1) * 0.5 * height;
    y2 = (1 + y2) * 0.5 * height;

    u0 /= z0;
    u1 /= z1;
    u2 /= z2;

    v0 /= z0;
    v1 /= z1;
    v2 /= z2;

    z0 = 1 / z0;
    z1 = 1 / z1;
    z2 = 1 / z2;

    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float xmax = max(max(ceil(x0), ceil(x1)), ceil(x2));

    float ymin = min(min(floor(y0), floor(y1)), floor(y2));
    float ymax = max(max(ceil(y0), ceil(y1)), ceil(y2));

    for (float y = ymin; y <= ymax; ++y) {
        for (float x = xmin; x <= xmax; ++x) {
            float alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            float beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            float gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1) {
                float s = texture.get_width() * (alpha * u0 + beta * u1 + gamma * u2);
                float t = texture.get_height() * (alpha * v0 + beta * v1 + gamma * v2);
                float z = 1 / (alpha * z0 + beta * z1 + gamma * z2);
                s = s * z;
                t = t * z;
                set_color(x, y, image, texture.get(round(s), round(t)));
            }
        }
    }
}

int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);

    draw_triangle_2d(image, 150, 180, 330, 230, 250, 350, blue);
    draw_triangle_2d(image, 350, 380, 380, 350, 400, 500, red);
    draw_triangle_2d_gouraurd(image, 20, 30, red, 180, 80, green, 100, 200, blue);
    
    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("triangles.tga");

    TGAImage image2(width, height, TGAImage::RGB);

    draw_triangle(image2, -48, -10, 82, 29, -15, 44, 13, 34, 114, blue);

    image2.flip_vertically();
    image2.write_tga_file("problem3.tga");

    TGAImage image3(width, height, TGAImage::RGB);

    draw_triangle_tex(image3, -48, -10, 82, 29, -15, 44, 13, 34, 114, 0, 0, 0, 1, 1, 0, "texture.tga");

    image3.flip_vertically();
    image3.write_tga_file("problem4_1.tga");

    TGAImage image4(width, height, TGAImage::RGB);

    draw_triangle_tex_corrected(image4, -48, -10, 82, 29, -15, 44, 13, 34, 114, 0, 0, 0, 1, 1, 0, "texture.tga");

    image4.flip_vertically();
    image4.write_tga_file("problem4_2.tga");
}