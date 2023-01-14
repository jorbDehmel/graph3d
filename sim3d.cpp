#include "sim3d.hpp"

/////////////////////////////////////////

Sim3D::Sim3D(const int W, const int H, bool (*Equation)(Point3D &Cur),
             SDL_Color (*ColorEquation)(const double &))
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_CreateWindowAndRenderer(W, H, SDL_WINDOW_OPENGL, &wind, &rend);

    equations.push_back(Equation);
    colorEquations.push_back(ColorEquation);

    rotation = Rotation(0, 0, 0);
    transpose = Point3D(W / 2, H / 2, 200);
    scale = 1;

    horizon.x = W / 2;
    horizon.y = H / 2;
    horizon.z = 100;

    return;
}

Sim3D::~Sim3D()
{
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wind);

    SDL_Quit();

    return;
}

/////////////////////////////////////////

void Sim3D::refresh()
{
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
    SDL_RenderClear(rend);

    vector<ColorWrapper> points;

    // construct plane lines
    if (doAxii)
    {
        for (double x = min.x; x < max.x; x += xSpacing)
        {
            Point3D toAdd = convertPoint(Point3D(x, 0, 0));
            points.push_back(ColorWrapper(toAdd, axisColor));
        }
        for (double y = min.y; y < max.y; y += ySpacing)
        {
            Point3D toAdd = convertPoint(Point3D(0, y, 0));
            points.push_back(ColorWrapper(toAdd, axisColor));
        }
        for (double z = min.z; z < max.z; z += ySpacing)
        {
            Point3D toAdd = convertPoint(Point3D(0, 0, z));
            points.push_back(ColorWrapper(toAdd, axisColor));
        }
    }

    // calculate points
    for (int eqInd = 0; eqInd < equations.size(); eqInd++)
    {
        Point3D pos;
        if (startingPositions.empty())
            pos = Point3D(0, 0, 0);
        else
            pos = startingPositions[eqInd % startingPositions.size()];

        int steps = 0;
        while (equations[eqInd](pos))
        {
            SDL_Color color = (colorEquations[eqInd](pos.z));

            if (pos.x > min.x && pos.x < max.x && pos.y > min.y && pos.y < max.y && pos.z > min.z && pos.z < max.z)
            {
                points.push_back(ColorWrapper(convertPoint(pos), color));
            }

            if (steps++ > 1'000'000)
            {
                throw runtime_error("Non-terminating equation.");
            }
        }
    }

    // Render points
    sort(points.begin(), points.end(), __sortFunction);

    for (int i = 0; i < points.size(); i++)
    {
        if (points[i].p.x < 0)
            continue;
        else if (points[i].p.y < 0)
            continue;
        else if (points[i].p.z < 0)
            continue;

        SDL_Color color = points[i].color;
        SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

        renderPoint(points[i].p);
    }

    SDL_RenderPresent(rend);

    return;
}

/////////////////////////////////////////

// convert point from graph coordinates to absolute rendering coordinates
Point3D Sim3D::convertPoint(const Point3D what)
{
    Point3D out;
    out.x = what.x * scale;
    out.y = what.y * scale;
    out.z = what.z * scale;

    rotate(out, Rotation(rotation.x + (M_PI / 2), rotation.y, rotation.z));

    out.x += transpose.x;
    out.y += transpose.y;
    out.z += transpose.z;

    return out;
}

void Sim3D::renderPoint(const Point3D what)
{
    SDL_FPoint projected = projectPoint(what);

    SDL_Rect toRend;
    toRend.x = projected.x - dotSize;
    toRend.y = projected.y - dotSize;
    toRend.w = toRend.h = dotSize * 2;

    SDL_RenderFillRect(rend, &toRend);

    return;
}

void Sim3D::screenshot(const char *where)
{
    int w, h;
    SDL_GetWindowSize(wind, &w, &h);

    SDL_Surface *sshot = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(rend, NULL, 0, sshot->pixels, sshot->pitch);
    SDL_SaveBMP(sshot, where);
    SDL_FreeSurface(sshot);

    return;
}

/////////////////////////////////////////
