#ifndef SCENE
#define SCENE

#include "Intersection.h"
#include "Sphere.h"
#include "Vector.h"
#include <iostream>

struct Scene {
    std::vector<Geometry*> geometries;
    bool indirect_lighting;

    Geometry* closest_geometry(const Ray &ray);
    Intersection intersect(const Ray &ray);

    Scene(std::vector<Geometry*> geometries, bool indirect_lighting) {
        this->geometries = geometries;
        this->indirect_lighting = indirect_lighting;
    }

    void addGeometry(Geometry *geometry) {
        geometries.push_back(geometry); 
    }
};

Geometry* Scene::closest_geometry(const Ray &ray) {
    double min_t = HUGE_VAL; // some arbitrary big number
    Geometry* closest_geometry;
    for (auto &geometry : geometries) {
        Intersection intersection = geometry->intersect(ray);
        if (intersection.occured && intersection.t < min_t) {
            min_t = intersection.t;
            closest_geometry = geometry;
        }
    }
    return closest_geometry;
}

// computes an intersection between ray and sphere
Intersection Scene::intersect(const Ray &ray) {
    Intersection intersection;
    Geometry* closest_geometry = Scene::closest_geometry(ray);

    // case when no intersection was found
    if (closest_geometry->R == 0) {
        intersection.occured = false;
    } 
    else {
        intersection.occured = true;
        intersection = closest_geometry->intersect(ray);
        intersection.albedo = closest_geometry->albedo;
    }
    return intersection;
}
#endif