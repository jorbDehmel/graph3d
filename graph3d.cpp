// 3D graphing system
// Jordan "Jorb" Dehmel, 2023
// jdehmel@outlook.com
// github.com/jorbDehmel

/////////////////////////////////////////

#include "graph3d.hpp"

char *FONT_PATH = "/usr/include/jgame3d/fonts/Open_Sans/static/OpenSans/OpenSans-Bold.ttf";
int FONT_POINTS = 12;
double LABEL_OFFSET = 5;
extern string TITLE = "Untitled graph";

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

    writer = new Writer(rend, FONT_PATH, FONT_POINTS);

    equations.push_back(Equation);
    colorEquations.push_back(ColorEquation);

    transpose = Point3D(W / 2, H / 2, 200);
    horizon = transpose;

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

    if (doLabels)
    {
        // xmin
        Point3D where(min.x - LABEL_OFFSET, 0, 0);
        SDL_FPoint whereProjected = projectPoint(convertPoint(where));
        writer->write("x=" + to_string(min.x), whereProjected.x, whereProjected.y, labelColor);

        // xmax
        where = Point3D(max.x + LABEL_OFFSET, 0, 0);
        whereProjected = SDL_FPoint(projectPoint(convertPoint(where)));
        writer->write("x=" + to_string(max.x), whereProjected.x, whereProjected.y, labelColor);

        // ymin
        where = Point3D(0, min.y - LABEL_OFFSET, 0);
        whereProjected = SDL_FPoint(projectPoint(convertPoint(where)));
        writer->write("y=" + to_string(min.y), whereProjected.x, whereProjected.y, labelColor);

        // ymax
        where = Point3D(0, max.y + LABEL_OFFSET, 0);
        whereProjected = SDL_FPoint(projectPoint(convertPoint(where)));
        writer->write("y=" + to_string(max.y), whereProjected.x, whereProjected.y, labelColor);

        // zmin
        where = Point3D(0, 0, min.z - LABEL_OFFSET);
        whereProjected = SDL_FPoint(projectPoint(convertPoint(where)));
        writer->write("z=" + to_string(min.z), whereProjected.x, whereProjected.y, labelColor);

        // zmax
        where = Point3D(0, 0, max.z + LABEL_OFFSET);
        whereProjected = SDL_FPoint(projectPoint(convertPoint(where)));
        writer->write("z=" + to_string(max.z), whereProjected.x, whereProjected.y, labelColor);
    }

    if (true)
    {
        writer->write(TITLE, 5, 5, labelColor);
    }

    // calculate points
    for (double x = min.x; x < max.x; x += xSpacing)
    {
        for (double y = min.y; y < max.y; y += ySpacing)
        {
            for (int eqIndex = 0; eqIndex < equations.size(); eqIndex++)
            {
                Point3D cur(x, y, (equations[eqIndex])(x, y));

                if (cur.z > min.z && cur.z < max.z)
                {
                    SDL_Color color = (colorEquations[eqIndex % colorEquations.size()])(cur.z);
                    points.push_back(ColorWrapper(convertPoint(cur), color));
                }
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
Point3D Graph3D::convertPoint(const Point3D what)
{
    Point3D out;
    out.x = what.x * scale.x;
    out.y = what.y * scale.y;
    out.z = what.z * scale.z;

    rotate(out, Rotation(rotation.x + (M_PI / 2), rotation.y, rotation.z));

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

void Graph3D::screenshot(const char *Where, const double &AtScale)
{
    int w, h;
    SDL_GetWindowSize(wind, &w, &h);

    SDL_Surface *unscaled = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(rend, NULL, 0, unscaled->pixels, unscaled->pitch);

    SDL_Surface *scaled = SDL_CreateRGBSurface(0, w * AtScale, h * AtScale, 32, 0, 0, 0, 0);
    SDL_BlitScaled(unscaled, NULL, scaled, NULL);

    SDL_SaveBMP(scaled, Where);

    SDL_FreeSurface(unscaled);
    SDL_FreeSurface(scaled);

    return;
}

/////////////////////////////////////////
