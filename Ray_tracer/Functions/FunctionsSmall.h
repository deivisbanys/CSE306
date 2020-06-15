#include "../Objects/Vector.h"

// clips number m such that it takes value in [min_range, max_range]
double clip(double m, double min_range, double max_range) {
    m = std::max(min_range, m);
    m = std::min(max_range, m);
    return m;
}

// applies gamma correction to a vector
Vector gammaCorrection(Vector vector, double gamma) {
    double inv_gamma = 1 / gamma;
    vector[0] = pow(vector[0], inv_gamma);
    vector[1] = pow(vector[1], inv_gamma);
    vector[2] = pow(vector[2], inv_gamma);
    return vector;
}