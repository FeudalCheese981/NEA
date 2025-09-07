#define _USE_MATH_DEFINES
#include <cmath>

#include "window.hpp"

int main()
{
	Window mwd = Window("Nea OpenGL", 720, 720, 200, 200);
	mwd.Initialize();
	mwd.RenderLoop();
	mwd.Terminate();

	return 0;
}