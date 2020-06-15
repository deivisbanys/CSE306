#include "svg_writer_for_polygons.cpp"
using namespace std;

// -------------------- SMALL FUNCTIONS --------------------

// generates a bounding box
Polygon generate_bounds(){
    Polygon bounds({Vector(0, 0), Vector(0, 1), 
                    Vector(1, 1), Vector(1, 0)});

    return bounds;
}

// generates n Vectors (x,y) with x,y ~ Uniform(0,1)
vector<Vector> generate_points(int n){
    vector<Vector> points;

    for (int i = 0; i < n; i++){
        float x = (float) rand() / RAND_MAX;
        float y = (float) rand() / RAND_MAX;

        points.push_back(Vector(x, y));
    }

    return points;
};

// generates n weights where each weight ~ Uniform(0,1)
vector<float> generate_weights(int n){

    float weight;
    vector<float> weights;

    for (int i = 0; i < n; i++){
        weight = (float) rand() / RAND_MAX;
        weights.push_back(weight);
    }

    return weights;
};

// -------------------- FUNCTIONS (Sutherland-Hodgman algorithm) --------------------

Vector intersect(Vector A, Vector B, vector<Vector> clipEdge, bool voronoi = false, vector<float> weights = {}){

    Vector u = clipEdge[0];
    Vector v = clipEdge[1];

    Vector N;
    double t;

    if (voronoi){
        N = (u + v) / 2;
        t = dot(N - A, u - v) / dot(B - A, u - v);

        // case of power diagrams
        if(weights.size()){
            N += (weights[0] - weights[1]) / (2 * pow(norm(u - v), 2)) * (v - u);
        }

    }
    else{
        N = Vector(v[1] - u[1], u[0] - v[0]);
        t = dot(u - A, N) / dot(B - A, N);
    }

    if (0 <= t && t <= 1) {
        Vector P = A + t * (B - A);
        return P;
    }

    return Vector();
}

bool is_inside(Vector P, vector<Vector> clipEdge, bool voronoi = false, vector<float> weights = {}){

    Vector u = clipEdge[0];
    Vector v = clipEdge[1];

    Vector N;
    bool inside;

    if (voronoi){
        N = (u + v) / 2;

        // case of power diagrams
        if(weights.size()){
            N += (weights[0] - weights[1]) / (2 * pow(norm(u - v), 2)) * (v - u);
        }
        inside = dot(P - N, v - u) < 0;
    }
    else{
        N = Vector(v[1] - u[1], u[0] - v[0]);
        inside = dot(P - u, N) <= 0;
    }

    return inside;
}

Polygon clip_Polygon(Polygon subjectPolygon, Polygon clipPolygon){

    Polygon outPolygon, emptyPolygon;

    #pragma omp parallel for
    for (int i = 0; i < clipPolygon.vertices.size(); i++){

        // Defining clipEdge
        Vector u = clipPolygon.vertices[i];
        Vector v = clipPolygon.vertices[(i > 0)? (i - 1) : (clipPolygon.vertices.size() - 1)];

        vector<Vector> clipEdge = {u, v};

        outPolygon = emptyPolygon;

        // Clip the subjectPolygon by a half-space
        for (int j = 0; j < subjectPolygon.vertices.size(); j++) {

            // Test the subject polygon edge with vertices (i-1, i)
            Vector curVertex = subjectPolygon.vertices[j];
            Vector prevVertex = subjectPolygon.vertices[(j > 0)? (j - 1) : (subjectPolygon.vertices.size() - 1)];

            // Compute intersection between the infinite line supported by clipEdge and edge (i-1, i)
            Vector intersection = intersect(prevVertex, curVertex, clipEdge);

            if (is_inside(curVertex, clipEdge)) {
                if (!is_inside(prevVertex, clipEdge)) {
                    // The subject polygon edge crosses the clip edge, and we leave the clipping area
                    outPolygon.add(intersection);
                }
                outPolygon.add(curVertex);
            }

            else if (is_inside(prevVertex, clipEdge)) {
                // The subject polygon edge crosses the clip edge, and we enter the clipping area
                outPolygon.add(intersection);
            }
        }

        subjectPolygon = outPolygon;
    }

    return outPolygon;
}

vector<Polygon> generate_voronoi_diagram(vector<Vector> points, Polygon bounds, vector<float> weights = {}) {

    vector<Polygon> voronoi_diagram;
    Polygon subjectPolygon;
    vector<float> weights_cur = {};

    for(int i = 0; i < points.size(); i++) {
        Vector Pi = points[i];
        subjectPolygon = bounds;

        for(int j = 0; j < points.size(); j++) {
            if(i == j) continue;

            Vector Pj = points[j];
            vector<Vector> clipEdge = {Pi, Pj};

            // case of power diagrams
            if (weights.size()){
                weights_cur = {weights[i], weights[j]};
            }
            
            Polygon outPolygon;

            for(int k = 0; k < subjectPolygon.vertices.size(); k++) {

                // Test the subject polygon edge with vertices (i-1, i)
                Vector curVertex = subjectPolygon.vertices[k];
                Vector prevVertex = subjectPolygon.vertices[(k > 0)? (k - 1) : (subjectPolygon.vertices.size() - 1)];

                // Compute intersection between the infinite line supported by clipEdge and edge (i-1, i)
                Vector intersection = intersect(prevVertex, curVertex, clipEdge, true, weights_cur);

                if (is_inside(curVertex, clipEdge, true, weights_cur)) {
                    if (!is_inside(prevVertex, clipEdge, true, weights_cur)) {
                        // The subject polygon edge crosses the clip edge, and we leave the clipping area
                        outPolygon.add(intersection);
                    }
                    outPolygon.add(curVertex);
                }

                else if (is_inside(prevVertex, clipEdge, true, weights_cur)) {
                    // The subject polygon edge crosses the clip edge, and we enter the clipping area
                    outPolygon.add(intersection);
                }
            }
            subjectPolygon = outPolygon;
        }
        voronoi_diagram.push_back(subjectPolygon);
    }
    return voronoi_diagram;
}