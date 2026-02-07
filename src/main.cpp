#include <core.hpp>

#include <Windows.h>

// internal structs
struct Win32Backbuffer {
	BITMAPINFO info;
	void* memory;
	int w; // width
	int h; // height
	int pitch;
};

// static global consts
static constexpr wchar window_class_name[] = L"rasterizer";
static constexpr wchar window_title[] = L"Rasterizer by WhoSayin52";

// static global vars;
Win32Backbuffer  global_win32_backbuffer;

// functions 
static LRESULT win32_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
static void win32_draw(HDC device_context, Win32Backbuffer* buffer);
static bool win32_init_backbuffer(Win32Backbuffer* buffer, int w, int h);

int WINAPI wWinMain(HINSTANCE process, HINSTANCE prev_, PWSTR cmd_args, int show_code) {
	(void)prev_, cmd_args, show_code;

	// creating and registering a window with OS
	WNDCLASS window_class{};
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = win32_procedure;
	window_class.hInstance = process;
	window_class.lpszClassName = window_class_name;

	RegisterClass(&window_class);

	// Creating an instance of our window class
	// Initializing our DIB global back buffer and using it to set window size 
	win32_init_backbuffer(&global_win32_backbuffer, 960, 540);
	RECT client_rect = { 0, 0, global_win32_backbuffer.w, global_win32_backbuffer.h };
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	AdjustWindowRect(&client_rect, style, 0);

	HWND window = CreateWindowEx(
		0,
		window_class_name,
		window_title,
		style,
		CW_USEDEFAULT,	// x
		CW_USEDEFAULT, // y
		client_rect.right - client_rect.left,	// width
		client_rect.bottom - client_rect.top,	// height
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


		HDC device_context = GetDC(window);
		win32_draw(device_context, &global_win32_backbuffer);
		ReleaseDC(window, device_context);
	}

	return 0;
}

static LRESULT win32_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {

	LRESULT result{};
	switch (message) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC device_context = BeginPaint(window, &ps);

		win32_draw(device_context, &global_win32_backbuffer);

		EndPaint(window, &ps);
		break;
	}
	default: {
		result = DefWindowProc(window, message, wparam, lparam);
	}
	}
	return result;
}

static void win32_draw(HDC device_context, Win32Backbuffer* buffer) {
	StretchDIBits(
		device_context,
		0, 0, buffer->w, buffer->h,	// dest
		0, 0, buffer->w, buffer->h,	// src
		buffer->memory, &buffer->info,
		DIB_RGB_COLORS, SRCCOPY
	);
}

static bool win32_init_backbuffer(Win32Backbuffer* buffer, int w, int h) {
	ASSERT(w >= 0 && h >= 0 && buffer->memory == nullptr);
	int bpp = 32; // bits per byte

	buffer->w = w;
	buffer->h = h;

	buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
	buffer->info.bmiHeader.biWidth = w;
	buffer->info.bmiHeader.biHeight = h;
	buffer->info.bmiHeader.biPlanes = 1;
	buffer->info.bmiHeader.biBitCount = (WORD)(bpp);
	buffer->info.bmiHeader.biCompression = BI_RGB;

	buffer->pitch = ALIGN4(w * bpp / 8);
	u64 memory_size = (u64)(buffer->pitch * buffer->h);
	buffer->memory = VirtualAlloc(0, memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	return buffer->memory != nullptr;
}
