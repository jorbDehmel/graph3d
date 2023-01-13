#include "graph3d.hpp"
using namespace std;

const double eq(const double &x, const double &y)
{
    return x * y;
}

int main()
{
    FOVScalar = 400;

    Graph3D g(1028, 1028, eq);
    g.scale = 5;
    g.axisColor.r = g.axisColor.g = g.axisColor.b = 255;

    g.min = Point3D(-20, -20, -20);
    g.max = Point3D(20, 20, 20);
    g.xSpacing = g.ySpacing = .1;

    int toWait = 10;

    bool isRunning = true;
    double stepSize = 5;
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
                case 27:
                    isRunning = false;
                    break;
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
                case 'p':
                    cout << "Saving screenshot...\n";
                    g.screenshot((string("screenshot_") + to_string(time(NULL)) + ".bmp").c_str());
                    break;
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

        g.rotation += Rotation(0.01, 0, 0.01);
    }

    return 0;
}
