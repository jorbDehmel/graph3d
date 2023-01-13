/////////////////////////////////////////

#include "graph3d.hpp"

/////////////////////////////////////////

SDL_Color __DefaultColorFunction(const double &in)
{
    Uint8 val = 255 / (1 + pow(2.718281821, -(in / 10)));

    SDL_Color out;
    out.a = 255;
    out.r = out.g = out.b = val;

    return out;
}

/////////////////////////////////////////

Graph3D::Graph3D(const int W, const int H, const double (*Equation)(const double &, const double &),
                 SDL_Color (*ColorEquation)(const double &))
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_CreateWindowAndRenderer(W, H, SDL_WINDOW_OPENGL, &wind, &rend);

    equation = Equation;
    colorEquation = ColorEquation;

    rotation = Rotation(0, 0, 0);
    transpose = Point3D(W / 2, H / 2, 200);
    scale = 1;

    horizon.x = W / 2;
    horizon.y = H / 2;
    horizon.z = 100;

    return;
}

Graph3D::~Graph3D()
{
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wind);

    SDL_Quit();

    return;
}

/////////////////////////////////////////

bool __sortFunction(const ColorWrapper &a, const ColorWrapper &b)
{
    return a.p.z > b.p.z;
}

void Graph3D::refresh()
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
    for (double x = min.x; x < max.x; x += xSpacing)
    {
        for (double y = min.y; y < max.y; y += ySpacing)
        {
            Point3D cur(x, y, equation(x, y));

            if (cur.z > min.z && cur.z < max.z)
            {
                SDL_Color color = colorEquation(cur.z);
                points.push_back(ColorWrapper{convertPoint(cur), color});
            }
        }
    }

    // Render points
    sort(points.begin(), points.end(), __sortFunction);

    for (int i = 0; i < points.size(); i++)
    {
        SDL_Color color = points[i].color;
        SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

        renderPoint(points[i].p);
    }

    SDL_RenderPresent(rend);

    return;
}

/////////////////////////////////////////

// convert point from graph coordinates to absolute rendering coordinates
Point3D Graph3D::convertPoint(const Point3D what)
{
    Point3D out;
    out.x = what.x * scale;
    out.y = what.y * scale;
    out.z = what.z * scale;

    rotate(out, rotation);

    out.x += transpose.x;
    out.y += transpose.y;
    out.z += transpose.z;

    return out;
}

void Graph3D::renderPoint(const Point3D what)
{
    SDL_FPoint projected = projectPoint(what);

    SDL_Rect toRend;
    toRend.x = projected.x - dotSize;
    toRend.y = projected.y - dotSize;
    toRend.w = toRend.h = dotSize * 2;

    SDL_RenderFillRect(rend, &toRend);

    return;
}

void Graph3D::screenshot(const char *where)
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
