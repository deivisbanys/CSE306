#ifndef VECTOR
#define VECTOR

#include <algorithm>
#include <cmath>
#include <vector>

struct Vector {
    explicit Vector(double x = 0., double y = 0., double z = 0.) {
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
    };
    Vector &operator+=(const Vector &b) {
        coords[0] += b[0];
        coords[1] += b[1];
        coords[2] += b[2];
        return *this;
    }
    Vector &operator-=(const Vector &b) {
        coords[0] -= b[0];
        coords[1] -= b[1];
        coords[2] -= b[2];
        return *this;
    }
    Vector &operator*=(double t) {
        coords[0] *= t;
        coords[1] *= t;
        coords[2] *= t;
        return *this;
    }
    Vector &operator/=(double t) {
        coords[0] /= t;
        coords[1] /= t;
        coords[2] /= t;
        return *this;
    }
    const double &operator[](int i) const { return coords[i]; }
    double &operator[](int i) { return coords[i]; }

  private:
    double coords[3];
};

Vector operator+(const Vector &a, const Vector &b) {
    return Vector(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

Vector operator-(const Vector &a, const Vector &b) {
    return Vector(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

Vector operator*(double t, const Vector &a) {
    return Vector(a[0] * t, a[1] * t, a[2] * t);
}

// we want t*a = a*t to hold
Vector operator*(const Vector &a, double t) {
    return Vector(a[0] * t, a[1] * t, a[2] * t);
}

// element-wise multiplication
Vector operator*(const Vector &a, const Vector &b) {
    return Vector(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

Vector operator/(const Vector &a, double t) {
    return Vector(a[0] / t, a[1] / t, a[2] / t);
}

double dot(const Vector &a, const Vector &b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

Vector cross(const Vector &a, const Vector &b) {
    return Vector(a[1] * b[2] - a[2] * b[1], 
                  a[2] * b[0] - a[0] * b[2],
                  a[0] * b[1] - a[1] * b[0]);
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

Vector min(const Vector &a, const Vector &b) {
    return Vector(std::min(a[0], b[0]), 
                  std::min(a[1], b[1]),
                  std::min(a[2], b[2]));
}

Vector max(const Vector &a, const Vector &b) {
    return Vector(std::max(a[0], b[0]), 
                  std::max(a[1], b[1]),
                  std::max(a[2], b[2]));
}

Vector abs(const Vector &a) {
    return Vector(std::abs(a[0]), std::abs(a[1]), std::abs(a[2]));
}

// returns an index of smallest coordinate
int minCoord(const Vector &a) {
    if (a[0] <= std::min(a[1], a[2])) {
        return 0;
    } 
    else if (a[1] <= a[2]) {
        return 1;
    }

    return 2;
}

#endif