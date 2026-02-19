
#include "./renderer/render.hpp"

#include <core/core.hpp>
#include <Windows.h>

// static global consts
constexpr wchar window_class_name[] = L"rasterizer";
constexpr wchar window_title[] = L"Rasterizer by WhoSayin52";
constexpr u32 win32_backbuffer_width = 1280; //960;
constexpr u32 win32_backbuffer_heigh = 720; //540;
constexpr usize permanent_memory_size = Memory::megabytes(1);
constexpr usize transient_memory_size = Memory::megabytes(1);

// internal structs
struct Win32_Backbuffer {
	BITMAPINFO info;
	void* memory;
	u32 w; // width
	u32 h; // height
	u32 pitch;
};

struct Win32_State {
	wchar exe_path[MAX_PATH * 4];
	wchar* exe_name;
};

// static global vars;
static Win32_Backbuffer global_win32_backbuffer;
static bool global_is_running;
static Event global_event = {
	.key = Key::NONE,
	.mouse_position = Vector2{}
};

// functions 
static LRESULT win32_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
static void win32_draw(HDC device_context, Win32_Backbuffer* buffer);
static bool win32_init_backbuffer(Win32_Backbuffer* buffer, u32 w, u32 h);
static void win32_get_exe_path(Win32_State* state);

int WINAPI wWinMain(HINSTANCE process, HINSTANCE prev_, PWSTR cmd_args, int show_code) {
	(void)prev_, cmd_args;

	// creating and registering a window with OS
	WNDCLASS window_class{};
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = win32_procedure;
	window_class.hInstance = process;
	window_class.lpszClassName = window_class_name;

	RegisterClass(&window_class);

	// Creating an instance of our window class
	// Initializing our DIB global back buffer and using it to set window size 
	win32_init_backbuffer(&global_win32_backbuffer, win32_backbuffer_width, win32_backbuffer_heigh);
	RECT client_rect = { 0, 0, (LONG)global_win32_backbuffer.w, (LONG)global_win32_backbuffer.h };
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

	// initializing win32 state
	Win32_State win32_state{};
	win32_get_exe_path(&win32_state);

	// initializing renderer memory
	void* total_renderer_memory = VirtualAlloc(
		0, permanent_memory_size + transient_memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE
	);
	assert(total_renderer_memory != nullptr);

	Renderer_Memory memory;
	Memory::arena_init(&memory.permanent, permanent_memory_size, total_renderer_memory);
	Memory::arena_init(&memory.transient, transient_memory_size, (byte*)total_renderer_memory + permanent_memory_size);

	// initializing renderer 
	wchar path_to_assets[MAX_PATH * 4];
	usize copy_range = win32_state.exe_name - win32_state.exe_path;
	wcsncpy_s(path_to_assets, array_count(path_to_assets), win32_state.exe_path, copy_range);
	wcscat_s(path_to_assets, array_count(path_to_assets), L"..\\asset\\");
	init_renderer(&memory, path_to_assets);

	// initializing vars needed for the main loop
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	LARGE_INTEGER previous;
	QueryPerformanceCounter(&previous);

	Canvas canvas;
	canvas.framebuffer = global_win32_backbuffer.memory;
	canvas.w = global_win32_backbuffer.w;
	canvas.h = global_win32_backbuffer.h;
	canvas.pitch = global_win32_backbuffer.pitch;
	canvas.origin = Vector2i{ (i32)canvas.w / 2, (i32)canvas.h / 2 };

	global_is_running = true;
	MSG message{};
	while (global_is_running) {
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);

		f32 delta_time = (f32)(current.QuadPart - previous.QuadPart)
			/ (f32)frequency.QuadPart;

		previous = current;

		render(&memory.transient, &canvas, global_event, delta_time);

		HDC device_context = GetDC(window);
		win32_draw(device_context, &global_win32_backbuffer);
		ReleaseDC(window, device_context);

		memset(
			global_win32_backbuffer.memory,
			0,
			Memory::align4(global_win32_backbuffer.h * global_win32_backbuffer.pitch)
		);
		global_event.key = Key::NONE;
	}

	return 0;
}
#include <iostream>
static LRESULT win32_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {

	switch (message) {
	case WM_CREATE: {
		global_event.mouse_position.x = 0;
		global_event.mouse_position.y = 0;
		return 0;
	}
	case WM_CLOSE: {
		global_is_running = false;
		return 0;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC device_context = BeginPaint(window, &ps);

		PatBlt(device_context, 0, 0, global_win32_backbuffer.w, global_win32_backbuffer.h, BLACKNESS);
		win32_draw(device_context, &global_win32_backbuffer);

		EndPaint(window, &ps);
		return 0;
	}
	case WM_MOUSEMOVE: {
		POINT point;
		GetCursorPos(&point);
		global_event.mouse_position.x = (f32)point.x;
		global_event.mouse_position.y = (f32)point.y;

		std::cout << point.x << ", " << point.y << '\n';

		return DefWindowProc(window, message, wparam, lparam);
	}
	case WM_KEYDOWN: {

		switch (wparam) {
		case VK_SPACE: {
			global_event.key = Key::SPACE;
			break;
		}
		case 'P': {
			global_event.key = Key::P;
			break;
		}
		case 'W': {
			global_event.key = Key::W;
			break;
		}
		case 'A': {
			global_event.key = Key::A;
			break;
		}
		case 'S': {
			global_event.key = Key::S;
			break;
		}
		case 'D': {
			global_event.key = Key::D;
			break;
		}
		default: {
			global_event.key = Key::NONE;
		}
		}
		return DefWindowProc(window, message, wparam, lparam);
	}
	default: {
		return DefWindowProc(window, message, wparam, lparam);
	}
	}
}

static void win32_draw(HDC device_context, Win32_Backbuffer* buffer) {
	StretchDIBits(
		device_context,
		0, 0, (int)buffer->w, (int)buffer->h,	// dest
		0, 0, (int)buffer->w, (int)buffer->h,	// src
		buffer->memory, &buffer->info,
		DIB_RGB_COLORS, SRCCOPY
	);
}

static bool win32_init_backbuffer(Win32_Backbuffer* buffer, u32 w, u32 h) {
	assert(w > 0 && h > 0 && buffer->memory == nullptr);
	u32 bpp = 32; // bits per byte

	buffer->w = w;
	buffer->h = h;

	buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
	buffer->info.bmiHeader.biWidth = (LONG)w;
	buffer->info.bmiHeader.biHeight = (LONG)h;
	buffer->info.bmiHeader.biPlanes = 1;
	buffer->info.bmiHeader.biBitCount = (WORD)(bpp);
	buffer->info.bmiHeader.biCompression = BI_RGB;

	buffer->pitch = (u32)Memory::align4(w * bpp / 8);
	u32 memory_size = buffer->pitch * buffer->h;
	buffer->memory = VirtualAlloc(0, memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	return buffer->memory != nullptr;
}

static void win32_get_exe_path(Win32_State* state) {
	SetLastError(0);
	DWORD file_path_size = GetModuleFileName(nullptr, state->exe_path, array_count(state->exe_path));
	assert(file_path_size > 0 && GetLastError() != ERROR_INSUFFICIENT_BUFFER);
	(void)file_path_size;
	state->exe_name = state->exe_path;
	for (wchar* scan = state->exe_path; *scan; ++scan) {
		if (*scan == L'\\') {
			state->exe_name = scan + 1;
		}
	}
}
