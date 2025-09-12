#include "window.hpp"

int main()
{
	Window mwd = Window("Nea OpenGL", 1280, 720, 200, 200);
	mwd.Initialize();
	mwd.RenderLoop();
	mwd.Terminate();

	return 0;
}