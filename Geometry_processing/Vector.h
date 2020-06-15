#ifndef VECTOR
#define VECTOR

#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

struct Vector {
    explicit Vector(double x = 0., double y = 0.) {
        coords[0] = x;
        coords[1] = y;
    };
    Vector &operator+=(const Vector &b) {
        coords[0] += b[0];
        coords[1] += b[1];
        return *this;
    }
    Vector &operator-=(const Vector &b) {
        coords[0] -= b[0];
        coords[1] -= b[1];
        return *this;
    }
    Vector &operator*=(double t) {
        coords[0] *= t;
        coords[1] *= t;
        return *this;
    }
    Vector &operator/=(double t) {
        coords[0] /= t;
        coords[1] /= t;
        return *this;
    }
    const double &operator[](int i) const { return coords[i]; }
    double &operator[](int i) { return coords[i]; }

  private:
    double coords[2];
};

Vector operator+(const Vector &a, const Vector &b) {
    return Vector(a[0] + b[0], a[1] + b[1]);
}

Vector operator-(const Vector &a, const Vector &b) {
    return Vector(a[0] - b[0], a[1] - b[1]);
}

Vector operator*(double t, const Vector &a) {
    return Vector(a[0] * t, a[1] * t);
}

// we want t*a = a*t to hold
Vector operator*(const Vector &a, double t) {
    return Vector(a[0] * t, a[1] * t);
}

// element-wise multiplication
Vector operator*(const Vector &a, const Vector &b) {
    return Vector(a[0] * b[0], a[1] * b[1]);
}

Vector operator/(const Vector &a, double t) {
    return Vector(a[0] / t, a[1] / t);
}

double dot(const Vector &a, const Vector &b) {
    return a[0] * b[0] + a[1] * b[1];
}

double norm(const Vector &a) { 
    return sqrt(dot(a, a)); 
}

Vector normalize(const Vector &a) {
    double a_normed = norm(a);
    if (a_normed == 0){
        return Vector();
    }
    return a / norm(a);
}

#endif