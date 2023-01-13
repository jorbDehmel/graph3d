#ifndef GRAPH3D_HPP
#define GRAPH3D_HPP

#include <jgame3d/jgame3d.hpp>
using namespace std;

SDL_Color __DefaultColorFunction(const double &in)
{
    Uint8 val = 255 / (1 + pow(2.718281821, -(in / 50)));

    SDL_Color out;
    out.a = 255;
    out.r = out.g = out.b = val;

    return out;
}

// A 3D graph (where X and Y are independant and Z is dependant)
class Graph3D
{
public:
    Graph3D(const int W, const int H, const double (*Equation)(const double &, const double &),
            SDL_Color (*ColorEquation)(const double &) = __DefaultColorFunction);
    ~Graph3D();

    void incRotation(const Rotation By);
    void incScale(const double By);

    void setRotation(const Rotation To);
    void setScale(const double To);

    void resetScale();
    void resetRotation();

    // Render to screen, but do not recalculate graph.
    void render();

    // Recalculate all points on the graph and render.
    void refresh();

    ////////////////////////////////

    Point3D min = Point3D(-10, -10, -10);
    Point3D max = Point3D(10, 10, 10);

    double xSpacing = 1, ySpacing = 1;

    bool doPlanes = true;
    bool doTicks = true;

    double dotSize = 4;

    ////////////////////////////////

    SDL_Renderer *rend;
    SDL_Window *wind;

protected:
    Slicer *slicer;

    const double (*equation)(const double &x, const double &y);
    SDL_Color (*colorEquation)(const double &z);

    Rotation rotation = Rotation(0, 0, 0);
    Point3D transpose = Point3D(0, 0, 0);
    double scale = 1;

private:
    Point3D convertPoint(const Point3D what);
    void addPoint(const Point3D what);
};

#endif
