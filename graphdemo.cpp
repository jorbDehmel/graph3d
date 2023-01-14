#include "graph3d.hpp"
using namespace std;

const double eq(const double &x, const double &y)
{
    return sqrt(100 - pow(x, 2) - pow(y, 2)) + 10 * sin(SDL_GetTicks() / 1000.);
}

const double eq2(const double &x, const double &y)
{
    return -eq(x, y);
}

int main()
{
    FOVScalar = 400;

    FONT_POINTS = 20;

    Graph3D g(1028, 1028, eq);
    g.equations.push_back(eq2);

    g.min.z = -20;
    g.max.z = 20;

    g.scale = 5;
    g.axisColor.r = g.axisColor.g = g.axisColor.b = 255;

    g.xSpacing = g.ySpacing = .1;

    int toWait = 10;

    bool isRunning = true;
    double stepSize = 5;
    double rotSize = 0.01;

    while (isRunning)
    {
        auto start = SDL_GetTicks();

        g.refresh();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == 27)
            {
                isRunning = false;
            }

            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                // Control
                case 27:
                    isRunning = false;
                    break;
                case 'p':
                    cout << "Saving screenshot...\n";
                    g.screenshot((string("screenshot_") + to_string(time(NULL)) + ".bmp").c_str());
                    break;

                // Transposition
                case 'q':
                    g.transpose += Point3D(0, 0, stepSize);
                    break;
                case 'e':
                    g.transpose += Point3D(0, 0, -stepSize);
                    break;
                case 'w':
                    g.transpose += Point3D(0, -stepSize, 0);
                    break;
                case 's':
                    g.transpose += Point3D(0, stepSize, 0);
                    break;
                case 'a':
                    g.transpose += Point3D(-stepSize, 0, 0);
                    break;
                case 'd':
                    g.transpose += Point3D(stepSize, 0, 0);
                    break;

                // Rotation
                case 'i':
                    g.rotation += Rotation(rotSize, 0, 0);
                    break;
                case 'k':
                    g.rotation += Rotation(-rotSize, 0, 0);
                    break;
                case 'j':
                    g.rotation += Rotation(0, -rotSize, 0);
                    break;
                case 'l':
                    g.rotation += Rotation(0, rotSize, 0);
                    break;
                case 'u':
                    g.rotation += Rotation(0, 0, rotSize);
                    break;
                case 'o':
                    g.rotation += Rotation(0, 0, -rotSize);
                    break;

                // Default obvs
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        auto end = SDL_GetTicks();
        if (end - start < toWait)
        {
            SDL_Delay(toWait - end + start);
        }
        else
        {
            cout << "Update took " << end - start << " ms.\n";
        }

        g.rotation += Rotation(0.01, 0.01, 0.01);
    }

    return 0;
}
