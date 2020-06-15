#include <chrono>
using namespace std::chrono;

#include "Functions/Functions.h"
#include "Libraries/Libraries.h"

auto start = high_resolution_clock::now();

int main() {

    // settings
    bool indirect_lighting = true;
    int NB_PATHS = 10;

    // initializing the scene
    Scene scene = initScene(indirect_lighting);

    // initializing the light vector
    Vector light = Vector(-10, 20, 40);

    // adding diffuse sphere to a scene
    Geometry* sphere1 = new Sphere(Vector(-15, 0, 10), 10, Vector(1., 1., 1.), false);
    scene.addGeometry(sphere1);

    // adding reflective sphere to a scene
    Geometry* sphere2 = new Sphere(Vector(+15, 0, 10), 10, Vector(1., 1., 1.), true);
    scene.addGeometry(sphere2);

    // adding triangle to a scene
    TriangleMesh* triangleMesh = new TriangleMesh;
    triangleMesh->readOBJ("Models/Triangle/triangle.obj");
    Geometry* triangle = static_cast<Geometry*>(triangleMesh);
    triangle->reflective = true;
    scene.addGeometry(triangle);

    // adding cat to a scene (CURRENTLY DOES NOT WORK)
    // TriangleMesh* catMesh = new TriangleMesh;
    // catMesh->readOBJ("Models/Cat/cat.obj");
    // Geometry* cat = static_cast<Geometry*>(catMesh);
    // scene.addGeometry(cat);

    // initializing the image
    Image img(512, 512, M_PI / 2.2, Vector(0, 0, 55));

    // looping over the image
    #pragma omp parallel for
    for (int i = 0; i < img.H; i++) {
        for (int j = 0; j < img.W; j++) {

            Vector V, pixelColor;
            double x, y;

            V[2] = img.Q[2] - img.W / (2 * tan(img.fov / 2));
            
            // antialising part
            for (int k = 0; k < NB_PATHS; k++) {
                boxMuller(1, x, y);
                V[0] = img.Q[0] + j + 0.5 - img.W / 2;
                V[1] = img.Q[1] - i - 0.5 + img.H / 2;

                Vector N = normalize(V - img.Q);
                Ray ray = Ray(img.Q, N);
                pixelColor += getColor(ray, 5, scene, light, 1e5);
            }

            pixelColor /= NB_PATHS;

            // applying gamma correction
            pixelColor = gammaCorrection(pixelColor, 2.2);

            // saving color into image grid while keeping it in range [0, 255]
            img.grid[(i * img.W + j) * 3 + 0] = clip(pixelColor[0] * 255, 0, 255);
            img.grid[(i * img.W + j) * 3 + 1] = clip(pixelColor[1] * 255, 0, 255);
            img.grid[(i * img.W + j) * 3 + 2] = clip(pixelColor[2] * 255, 0, 255);
        }
    }
    
    // saving the image
    stbi_write_png("Images/TD3/triangle.png", img.W, img.H, 3, &img.grid[0], 0);

    // printing the running time
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "Time taken by function: " << duration.count() << " ms" << std::endl;

    return 0;
}