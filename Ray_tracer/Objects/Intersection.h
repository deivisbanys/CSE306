#ifndef INTERSECTION
#define INTERSECTION

#include "Vector.h"

struct Intersection {
    bool occured;  // indicator if an intersection occured
    double t;      // intersection parameters
    Vector P;      // intersection point P
    Vector N;      // unit normal N at P
    Vector albedo; // color
};
#endif