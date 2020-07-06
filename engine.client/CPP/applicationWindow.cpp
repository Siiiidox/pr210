#include "applicationWindow.h"
#include "appinfo.h"


LRESULT WINAPI WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_QUIT:
	//NO Break to bleed in Close
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		break;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void ApplicationWindow::Init(AnsiString applicationName, int width, int height)
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
		MessageBoxA(NULL, "FUNZT NET!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}
	this->hwnd = CreateWindowEx(
		0,
		className,
		applicationName,
		WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU,
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
		MessageBoxA(NULL, "FUNZT!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}
	this->Show();
}

void ApplicationWindow::Show()
{
	ShowWindow(hwnd, SW_SHOW);
}

void ApplicationWindow::Hide()
{
	ShowWindow(hwnd, SW_HIDE);
}

void ApplicationWindow::Maximize()
{
	ShowWindow(hwnd, SW_MAXIMIZE);
}

void ApplicationWindow::Minimize()
{
	ShowWindow(hwnd, SW_MINIMIZE);
}

void ApplicationWindow::Resize()
{
}

bool ApplicationWindow::MessagePump()
{
	MSG msg = {};
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

