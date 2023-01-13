#include "graph3d.hpp"
using namespace std;

const double eq(const double &x, const double &y)
{
    return x + y;
}

int main()
{
    Graph3D g(512, 512, eq);
    g.render();

    for (int i = 0; i < 1000; i++)
    {
        SDL_Delay(10);
    }

    return 0;
}
