#include "SoundMaker.h"
#include "MainWindow.h"

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR pCmdLine, int nCmdShow
) {
	win.setup();
	win.run();
}
