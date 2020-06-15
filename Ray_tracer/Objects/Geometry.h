#ifndef GEOMETRY
#define GEOMETRY

#include "Intersection.h"
#include "Ray.h"

struct Geometry{
    double R = 0;            // radius R
    Vector C;                // center C
    Vector albedo;           // RGB color
    bool reflective = false; // indicator if a geometry is reflective

    virtual Intersection intersect(const Ray &ray) = 0;
};
#endif