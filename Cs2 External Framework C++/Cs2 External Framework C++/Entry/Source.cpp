#include "../Util/include.h"
#include "../Overlay/overlay.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	Logger::CreateConsole();
	mem->GetHandle("cs2.exe");
	mem->clientDLL = mem->GetModuleAddress("client.dll");

	//if (!mem->clientDLL)
	//	exit(1);

	Logger::Log("[INFO] Found Client at ->  ", mem->clientDLL);

	std::thread([]() {
		reader.startCachingLoop(mem->clientDLL);
		}).detach();

	ExternalOverlay overlay;

	overlay.setWindowClass(hInstance, L"External C++ Framework");

	overlay.createWindow();

	overlay.makeFrameIntoClientArea();

	overlay.makeDeviceAndSwapChain();

	overlay.initWindow();

	overlay.renderLoop();
}