#include "AppWindow.h"
#include "appinfo.h"
LRESULT WINAPI WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_QUIT:
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		break;
		
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AppWindow::Init(AnsiString name, int width, int height)
{
	AnsiString className = "WinClass";

	WNDCLASSEX wc = {};
	wc.hInstance = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_APPICON));
	wc.lpfnWndProc = WinProc;
	wc.cbSize = sizeof(wc);
	wc.lpszClassName = className;
	

	if (FAILED(RegisterClassEx(&wc)))
	{
		MessageBoxA(NULL, "HAT NICHT FUNZT", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}
	this->hwnd = CreateWindowEx(
		0,
		className,
		name,
		WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		wc.hInstance,
		NULL
	);
	if (this->hwnd == NULL)
	{
		MessageBoxA(NULL, "HAT NICHT FUNZT", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}
	this->Show();
}

void AppWindow::Show()
{
	ShowWindow(hwnd, SW_SHOW);
}

void AppWindow::Hide()
{
	ShowWindow(hwnd, SW_HIDE);
}

void AppWindow::Maximize()
{
	ShowWindow(hwnd, SW_MAXIMIZE);
}

void AppWindow::Minimize()
{
	ShowWindow(hwnd, SW_MINIMIZE);
}

void AppWindow::Resize()
{

}

bool AppWindow::MessagePump()
{
	MSG msg = { };
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}
