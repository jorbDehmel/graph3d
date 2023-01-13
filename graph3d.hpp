#ifndef GRAPH3D_HPP
#define GRAPH3D_HPP

#include <jgame3d/jgame3d.hpp>

// Default color mapping function corrolating z-values to color
SDL_Color __DefaultColorFunction(const double &in);

// A light wrapper for remembering the color of a point
struct ColorWrapper
{
    ColorWrapper(Point3D P, SDL_Color Color) : p(P), color(Color) {}
    Point3D p;
    SDL_Color color;
};

// For sorting lists of ColorWrappers to be in proper render order
bool __sortFunction(const ColorWrapper &a, const ColorWrapper &b);

// A 3D graph (where X and Y are independant and Z is dependant)
class Graph3D
{
public:
    Graph3D(const int W, const int H, const double (*Equation)(const double &, const double &),
            SDL_Color (*ColorEquation)(const double &) = __DefaultColorFunction);
    ~Graph3D();

    // Recalculate all points on the graph and render.
    void refresh();

    // Save a screenshot locally
    void screenshot(const char *where);

    ////////////////////////////////

    Point3D min = Point3D(-10, -10, -10);
    Point3D max = Point3D(10, 10, 10);

    double xSpacing = 1, ySpacing = 1;

    bool doAxii = true;
    bool doTicks = true;

    SDL_Color axisColor = SDL_Color{255, 255, 255, 255};
    SDL_Color backgroundColor = SDL_Color{0, 0, 0, 0};

    double dotSize = 4;

    Rotation rotation = Rotation(0, 0, 0);
    Point3D transpose = Point3D(0, 0, 0);
    double scale = 1;

protected:
    const double (*equation)(const double &x, const double &y);
    SDL_Color (*colorEquation)(const double &z);

    SDL_Renderer *rend;
    SDL_Window *wind;

private:
    Point3D convertPoint(const Point3D what);
    void renderPoint(const Point3D what);
};

#endif
