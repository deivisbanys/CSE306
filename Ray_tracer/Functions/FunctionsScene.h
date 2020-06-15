#include "../Objects/Objects.h"
#include <random>

static std::default_random_engine engine(10); // random seed = 10
static std::uniform_real_distribution<double> uniform(0, 1);

// initializes empty box (i.e. a scene with 6 walls)
Scene initScene(bool indirect_lighting) {
    Geometry* sphere_red = new Sphere(Vector(0, 1000, 0), 940, Vector(1, 0, 0), false);
    Geometry* sphere_green = new Sphere(Vector(0, 0, -1000), 940, Vector(0, 1, 0), false);
    Geometry* sphere_blue = new Sphere(Vector(0, -1000, 0), 990, Vector(0, 0, 1), false);
    Geometry* sphere_pink = new Sphere(Vector(0, 0, 1000), 940, Vector(1, 0, 1), false);
    Geometry* sphere_yellow = new Sphere(Vector(1000, 0, 0), 940, Vector(1, 1, 0), false);
    Geometry* sphere_cyan = new Sphere(Vector(-1000, 0, 0), 940, Vector(0, 1, 1), false);

    std::vector<Geometry*> geometries;

    geometries.push_back(sphere_red);
    geometries.push_back(sphere_green);
    geometries.push_back(sphere_blue);
    geometries.push_back(sphere_pink);
    geometries.push_back(sphere_yellow);
    geometries.push_back(sphere_cyan);

    return Scene(geometries, indirect_lighting);
}

void boxMuller(double stdev, double &x, double &y) {
    double r1 = uniform(engine);
    double r2 = uniform(engine);
    x = sqrt(-2 * log(r1)) * cos(2 * M_PI * r2) * stdev;
    y = sqrt(-2 * log(r1)) * sin(2 * M_PI * r2) * stdev;
}

Vector randomCos(const Vector &N) {
    double r1 = uniform(engine);
    double r2 = uniform(engine);
    double x = cos(2 * M_PI * r1) * sqrt(1 - r2);
    double y = sin(2 * M_PI * r1) * sqrt(1 - r2);
    double z = sqrt(r2);

    // detecting the index (coordinate) of smallest
    // element of N in absolute value
    int min_coord = minCoord(abs(N));

    Vector T1;
    if (min_coord == 0) {
        T1 = Vector(0, N[2], -N[1]);
    } else if (min_coord == 1) {
        T1 = Vector(N[2], 0, -N[0]);
    } else {
        T1 = Vector(N[1], -N[0], 0);
    }

    T1 = normalize(T1);
    Vector T2 = cross(N, T1);

    return x * T1 + y * T2 + z * N;
}

Vector getColor(const Ray &ray, int ray_depth, Scene scene, Vector S, double I) {
    if (ray_depth < 0) {
        return Vector(0., 0., 0.);
    }
    Geometry* closest_geometry = scene.closest_geometry(ray);
    Intersection intersection = closest_geometry->intersect(ray);

    if (intersection.occured) {
        double eps = 1e-4; //some arbitrary epsilon

        Vector N = intersection.N;
        Vector P = intersection.P + eps * N; // to get rid of numerical precision issues

        // handling mirror surfaces
        if (closest_geometry->reflective) {
            Ray reflected_ray = Ray(P, ray.u - (2 * dot(ray.u, N)) * N);
            return getColor(reflected_ray, ray_depth - 1, scene, S, I);
        }

        // handling diffuse surfaces
        else {
            Vector Lo;

            // adding direct lighting
            double d = norm(S - P);
            Vector w = normalize(S - P);
            Ray ray = Ray(S, Vector(0, 0, 0) - w);

            Intersection intersection = scene.intersect(ray);
            bool V_p = (!intersection.occured || intersection.t > d);
            Vector ro = intersection.albedo;

            Lo = I / (4 * M_PI * pow(d, 2)) * ro / M_PI * V_p * std::max(dot(N, w), 0.);

            // adding indirect lighting
            if (scene.indirect_lighting) {
                Ray randomRay = Ray(P, randomCos(N));
                Lo += closest_geometry->albedo * getColor(randomRay, ray_depth - 1, scene, S, I);
            }
            return Lo;
        }
    }

    return Vector();
}