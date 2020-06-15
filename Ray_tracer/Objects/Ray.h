#ifndef RAY
#define RAY

#include "Vector.h"

struct Ray {
    Vector O; // origin vector
    Vector u; // unit direction

    Ray(Vector O, Vector u) {
        this->O = O;
        this->u = u;
    }
};
#endif