#pragma once
#include "types.h"
#include <Windows.h>

class ApplicationWindow
{
private:
	HWND hwnd;
public:
	void Init(AnsiString applicationName, int width, int height);
	void Show();
	void Hide();
	void Maximize();
	void Minimize();
	void Resize();
	bool MessagePump();
	bool GetClientSize(ui32& width, ui32& height);
	ui64 GetHandle();
};