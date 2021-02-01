#include "WindowHeader.h"
#include <iostream>

LRESULT CALLBACK WindowProcc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool SetupWindow(HINSTANCE instance, UINT width, UINT height, int nCmdShow, HWND& window)
{
	const wchar_t CLASS_NAME[] = L"Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProcc;
	wc.hInstance = instance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	window = CreateWindowEx(0, CLASS_NAME, L"Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, width, height, nullptr, nullptr, instance, nullptr);

	if (window == nullptr)
	{
		std::cerr << "HWND was " << GetLastError() << std::endl;
		return false;
	}

	ShowWindow(window, nCmdShow);

	return true;
}
