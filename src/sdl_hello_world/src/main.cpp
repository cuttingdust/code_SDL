#include "Display.h"

#include <iostream>


int main(int argc, char* argv[])
{
    Display display(800, 600, "My Display");

    while (!display.isClosed())
    {
        display.clear(0.f, 0.15f, 0.3f, 1.f);
        display.update();
    }

    getchar();
    return 0;
}
