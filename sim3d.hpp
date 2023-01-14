#ifndef SIM3D_HPP
#define SIM3D_HPP

#include "./graph3d.hpp"
using namespace std;

// A 3D sim (fully independant XYZ)
// Very useful for recursive 3D functions
class Sim3D
{
public:
    Sim3D(const int W, const int H, bool (*Equation)(Point3D &Cur),
          SDL_Color (*ColorEquation)(const double &) = __DefaultColorFunction);
    ~Sim3D();

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

    vector<bool (*)(Point3D &cur)> equations;
    vector<SDL_Color (*)(const double &z)> colorEquations;
    vector<Point3D> startingPositions;

protected:
    Writer *writer;
    SDL_Renderer *rend;
    SDL_Window *wind;

private:
    Point3D convertPoint(const Point3D what);
    void renderPoint(const Point3D what);
};

#endif
