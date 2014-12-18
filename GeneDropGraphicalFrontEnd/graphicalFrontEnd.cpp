#include "UserInterface.h"

#ifdef _WIN32
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, wchar_t*,
	int nCmdShow) {
#else
int main() {
#endif

	UserInterface app;
	app.run();

	return 0;
}
