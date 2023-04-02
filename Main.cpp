#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SolarSystem.h"
#include <Windows.h>
#include <iostream>

int main(int argc, char* argv[])
{
	const int showFullscreen = MessageBox(nullptr, L"Would you like to run in fullscreen mode?", L"Fullscreen", MB_YESNOCANCEL);

	SolarSystem Window;
	switch (showFullscreen)
	{
	case IDYES:
		if (!Window.createWindow(800, 600, "Solar System Simulator ", true))
		{
			return 1;
		}
		break;
	case IDNO:
		if (!Window.createWindow(800, 600, "Solar System Simulator ", false))
		{
			return 1;
		}
		break;
	case IDCANCEL:
		return 0;
		break;
	}
	//if (!Window.createWindow(800, 600, "Solar System Simulator ", showFullscreen))
	//{
	//	return 1;
	//}
	Window.runApp();

	return 0;
}
