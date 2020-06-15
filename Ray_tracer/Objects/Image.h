#ifndef IMAGE
#define IMAGE

#include "Vector.h"

struct Image {
    double W;   // width
    double H;   // height
    double fov; // horizontal field of view
    Vector Q;   // center
    std::vector<unsigned char> grid;

    Image(double W, double H, double fov, Vector Q) {
        this->W = W;
        this->H = H;
        this->fov = fov;
        this->Q = Q;
        this->grid = initGrid();
    }

    std::vector<unsigned char> initGrid() {
        std::vector<unsigned char> grid(W * H * 3);
        return grid;
    }
};
#endif