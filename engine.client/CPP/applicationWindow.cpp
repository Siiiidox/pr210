#include <iostream>

#include "applicationWindow.h"
#include "appinfo.h"


LRESULT WINAPI WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_QUIT:
	//No Break to bleed in Close
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
	wc.cbSize = sizeof(wc);
	wc.hInstance = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_APPICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = WinProc;
	wc.lpszClassName = className;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 

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

bool ApplicationWindow::GetClientSize(ui32& width, ui32& height)
{
	RECT rect = {};
	if (GetClientRect(hwnd, &rect))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
		return true;
	}
	else
	{
		printf("Caught Error: GetClientRect() FAILED!\n");
		return false;
	}
}

ui64 ApplicationWindow::GetHandle()
{
	return reinterpret_cast<ui64>(this->hwnd);
}

