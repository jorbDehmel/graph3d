#include "graph3d.hpp"

Graph3D::Graph3D(const int W, const int H, const double (*Equation)(const double &, const double &),
                 SDL_Color (*ColorEquation)(const double &))
{
    SDL_CreateWindowAndRenderer(W, H, SDL_WINDOW_OPENGL, &wind, &rend);

    equation = Equation;
    slicer = new Slicer(rend);

    rotation = Rotation(0, 0, 0);
    transpose = Point3D(0, 0, 0);
    scale = 1;

    render();

    return;
}

Graph3D::~Graph3D()
{
    delete slicer;

    return;
}

void Graph3D::incRotation(const Rotation By)
{
    rotation.x += By.x;
    rotation.y += By.y;
    rotation.z += By.z;

    return;
}

void Graph3D::incScale(const double By)
{
    scale += By;

    return;
}

void Graph3D::setRotation(const Rotation To)
{
    rotation = To;

    return;
}

void Graph3D::setScale(const double To)
{
    scale = To;

    return;
}

void Graph3D::resetScale()
{
    scale = 0;

    return;
}

void Graph3D::resetRotation()
{
    rotation.x = rotation.y = rotation.z = 0;

    return;
}

void Graph3D::render()
{
    slicer->render();

    return;
}

void Graph3D::refresh()
{
    // Clear slicer
    slicer->models.clear();

    // construct plane lines, add to slicer

    // calculate points, add to slicer as polygons
    for (double x = min.x; x < max.x; x += xSpacing)
    {
        for (double y = min.y; y < max.y; x += ySpacing)
        {
            Point3D toAdd(x, y, equation(x, y));
            addPoint(toAdd);
        }
    }

    // call slicer to render
    render();

    return;
}

void Graph3D::addPoint(const Point3D what)
{
    // get scaled point from what
    Point3D scaled = convertPoint(what);

    // Create cube to represent point
    Model temp;
    SDL_Color color = colorEquation(what.z);
    createCube(temp, color, dotSize * 2);

    // Adjust to be about origin
    move(temp, Point3D(-dotSize, -dotSize, -dotSize));

    // Move to be at position of scaled point
    move(temp, scaled);

    // Add to slicer
    slicer->models.push_back(temp);

    return;
}

Point3D convertPoint(const Point3D what)
{
    // convert point from graph coordinated to absolute rendering coordinates
    return what;
}
