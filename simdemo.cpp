#include "sim3d.hpp"
using namespace std;

bool eq(Point3D &p)
{
    p.x += 1;
    p.y = -p.x;

    return (p.x < 20);
}

int main()
{
    FOVScalar = 400;

    Sim3D g(1028, 1028, eq);

    g.min = Point3D(-20, -20, -20);
    g.max = Point3D(20, 20, 20);

    g.scale = 5;
    g.axisColor.r = g.axisColor.g = g.axisColor.b = 255;

    g.xSpacing = g.ySpacing = .1;

    int toWait = 10;

    bool isRunning = true;
    double stepSize = 5;
    double rotSize = 0.01;

    g.startingPositions.push_back(Point3D(0, 0, 0));

    while (isRunning)
    {
        auto start = SDL_GetTicks();

        g.startingPositions[g.startingPositions.size() - 1] += Point3D(0, .1, 0);

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
