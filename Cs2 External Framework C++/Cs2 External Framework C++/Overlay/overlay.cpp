#include "overlay.h"

LRESULT Wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return 0;
	}
	switch (msg) {
	case WM_SYSCOMMAND: {
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ExternalOverlay::setWindowClass(HINSTANCE hInstance, LPCWSTR windowname) {
	this->hInstance = hInstance;

	wndClass.cbSize = sizeof(WNDCLASSEXA);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = (WNDPROC)Wndproc;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpszClassName = windowname;

	RegisterClassExW(&wndClass);
};


void ExternalOverlay::createWindow() {
	HWND window = nullptr;

	int winx = GetSystemMetrics(SM_CXSCREEN);
	int winy = GetSystemMetrics(SM_CYSCREEN);

	window = CreateWindowExW(NULL, wndClass.lpszClassName, wndClass.lpszClassName, WS_POPUP, 0, 0, winx, winy, 0, 0, wndClass.hInstance, 0);
	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

	this->window = window;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
}


void ExternalOverlay::makeFrameIntoClientArea() {

	RECT clientArea{};
	GetClientRect(window, &clientArea);

	RECT windowArea{};
	GetWindowRect(window, &windowArea);

	POINT diff{};
	ClientToScreen(window, &diff);

	const MARGINS margins{
		windowArea.left + (diff.x - windowArea.left),
		windowArea.top + (diff.y - windowArea.top),
		clientArea.right,
		clientArea.bottom
	};

	DwmExtendFrameIntoClientArea(window, &margins);
}


void ExternalOverlay::makeDeviceAndSwapChain() {

	swapChain.BufferDesc.RefreshRate.Numerator = 240;
	swapChain.BufferDesc.RefreshRate.Denominator = 1;
	swapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChain.SampleDesc.Count = 1;
	swapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChain.BufferCount = 2;
	swapChain.OutputWindow = window;
	swapChain.Windowed = TRUE;
	swapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChain.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChain, &loadedSwapChain, &device, &loadedLevel, &deviceContext);

	loadedSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	if (backBuffer) {
		device->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
		backBuffer->Release();
	}
};

void ExternalOverlay::initWindow() {
	ShowWindow(window, SW_SHOW);
	UpdateWindow(window);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	ImFont* arialFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyhbd.ttc", 24.f);
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, deviceContext);
}

void ExternalOverlay::renderLoop() {
	bool state = true;
	bool check = false;

	while (state) {
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			menutoggle = !menutoggle;
		}

		MSG msg;
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				state = false;
			}

			if (!state) {
				break;
			}
		}

		if (GetAsyncKeyState(VK_END)) {
			this->destroyWindow();
			exit(0);
		}

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (this->menutoggle) {
			this->renderMenu();
			SetWindowLong(this->window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
		}
		else {
			SetWindowLong(this->window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
		}

		ImGui::EndFrame();
		ImGui::Render();

		float color[4]{ 0.f,0.f ,0.f ,0.f };
		deviceContext->OMSetRenderTargets(1, &renderTargetView, 0);
		deviceContext->ClearRenderTargetView(renderTargetView, color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		loadedSwapChain->Present(0, 0);

		if (!check) {
			check = !check;
		}
	}
}

inline void ExternalOverlay::renderMenu() {
	ImGui::Begin("External C++ Framework");
	ImGui::Text("Test text");
	if (ImGui::Button("Test button")) MessageBoxA(0, "Button Clicked!", "Info", MB_OK);
	ImGui::End();
}