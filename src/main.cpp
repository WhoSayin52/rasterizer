#include <core.hpp>

#include <Windows.h>

// static vars
static constexpr wchar window_class_name[] = L"rasterizer";
static constexpr wchar window_title[] = L"Rasterizer by WhoSayin52";

LRESULT window_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

int WINAPI wWinMain(HINSTANCE process, HINSTANCE prev_, PWSTR cmd_args, int show_code) {
	(void)prev_, cmd_args, show_code;

	// creating and registering a window with OS
	WNDCLASS window_class{};
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = window_procedure;
	window_class.hInstance = process;
	window_class.lpszClassName = window_class_name;

	RegisterClass(&window_class);

	// Creating an instance of our window class
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	HWND window = CreateWindowEx(
		0,
		window_class_name,
		window_title,
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr,
		process,
		nullptr
	);

	if (window == nullptr) {
		return 1;
	}

	ShowWindow(window, show_code);

	bool is_running = true;
	MSG message{};
	while (is_running) {
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return 0;
}

LRESULT window_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	return DefWindowProc(window, message, wparam, lparam);
}
