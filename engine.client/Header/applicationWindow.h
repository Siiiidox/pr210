#pragma once
#include "types.h"
#include <Windows.h>

class ApplicationWindow
{
private:
	/*!
	*	@brief variable to store the hwnd
	*/
	HWND hwnd;
public:
	/*!
	*	@brief Initialize the application window
	*	@param required the ApplicationName as String
	*	@param required the width as Integer
	*	@param required the height as Integer
	*	@return void
	*/
	void Init(AnsiString applicationName, int width, int height);
	/*!
	*	@brief Shows the Application Window
	*	@return void
	*/
	void Show();
	/*!
	*	@brief Hide the Application Window
	*	@return void
	*/
	void Hide();
	/*!
	*	@brief Maximize the Application Window
	*	@return void
	*/
	void Maximize();
	/*!
	*	@brief Minimize the Application Window
	*	@return void
	*/
	void Minimize();
	/*!
	*	@brief Handles the WindowsMessagePump
	*	@return boolean if the application still runnning
	*/
	bool MessagePump();
	/*!
	*	@brief Get the Size of the hwnd
	*	@param a variable to store the width
	*	@param a variable to store the height
	*	@return boolean if the function failed
	*/
	bool GetClientSize(ui32& width, ui32& height);
	/*!
	*	@brief get the Handle in ui64 of hwnd
	*	@return ui64 of hwnd
	*/
	ui64 GetHandle();
};