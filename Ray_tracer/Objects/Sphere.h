#ifndef SPHERE
#define SPHERE

#include "Geometry.h"
#include "Vector.h"
#include <string>

struct Sphere : public Geometry {
    Intersection intersect(const Ray &ray);

    Sphere(Vector C, double R, Vector albedo, bool reflective) {
        this->C = C;
        this->R = R;
        this->albedo = albedo;
        this->reflective = reflective;
    }
};

// computes an intersection between ray and sphere
Intersection Sphere::intersect(const Ray &ray) {
    Intersection intersection;
    Vector O_C = ray.O - C; // O_C := O - C
    double delta = pow(dot(ray.u, O_C), 2) - (dot(O_C, O_C) - pow(R, 2));
    intersection.occured = delta >= 0;

    if (intersection.occured) {
        double t_1 = dot(ray.u, -1 * O_C) - sqrt(delta);
        double t_2 = dot(ray.u, -1 * O_C) + sqrt(delta);

        if (t_2 < 0) {
            intersection.occured = false;
        }

        else {
            if (t_1 >= 0) {
                intersection.t = t_1;
            } else {
                intersection.t = t_2;
            }
        }
    }
    intersection.P = ray.O + intersection.t * ray.u;
    intersection.N = normalize(intersection.P - C);

    return intersection;
}
#endif