#include "Functions.cpp"

int main() {

    // generating bounding box
    Polygon bounds = generate_bounds();

    // --------------- testing Sutherland-Hodgman algorithm ---------------
    // Polygon subjectPolygon({Vector(0.2, 0.2), Vector(0.2, 0.8), Vector(0.4, 0.8), 
    //                         Vector(0.4, 0.4), Vector(0.8, 0.4), Vector(0.8, 0.2)});
    // Polygon clipPolygon({Vector(0.1, 0.1), Vector(0.1, 0.6), 
    //                      Vector(0.6, 0.6), Vector(0.6, 0.1)});
    // 
    // Polygon clipped_polygon = clip_Polygon(subjectPolygon, clipPolygon);
    
    // save_svg({subjectPolygon, clipPolygon}, "Images/clip.svg");
    // save_svg({clipped_polygon}, "Images/clipped.svg");
    
    // --------------- testing Voronoi diagram ---------------
    // vector<Vector> points = generate_points(1000);
    // vector<Polygon> voronoi_diagram = generate_voronoi_diagram(points, bounds);
    // save_svg(voronoi_diagram, "Images/voronoi_diagram_100.svg");

    // --------------- testing power diagram ---------------
    // vector<Vector> points = generate_points(1000);
    // vector<float> weights = generate_weights(1000);

    // vector<Polygon> power_diagram = generate_voronoi_diagram(points, bounds, weights);
    // save_svg(power_diagram, "Images/power_diagram_1000_sin.svg");

    return 0;
}