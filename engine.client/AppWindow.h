#pragma once
#include "types.h"
#include <Windows.h>
class AppWindow
{
private:
	HWND hwnd;
public:
	void Init(AnsiString name, int width, int height);
	void Show();
	void Hide();
	void Maximize();
	void Minimize();
	void Resize();
	bool MessagePump();
};