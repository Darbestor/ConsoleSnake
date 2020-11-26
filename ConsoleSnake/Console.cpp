//
// Copyright (C) Microsoft. All rights reserved.
//
#include "Console.h"
#define DEFINE_CONSOLEV2_PROPERTIES

#define ESC "\x1b"
#define CSI "\x1b["
#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 40

Console::Console(short width, short height)
{
	windowHeight = height;
	windowWidth = width;
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (consoleHandle == INVALID_HANDLE_VALUE)
	{
		throw;
	}
}

bool Console::SetupConsole()
{
	bool fSuccess = EnableVTMode();
	FixConsoleSize();
	fSuccess = SetConsoleBuffer();
	return fSuccess;
}

HANDLE Console::GetConsoleHandle()
{
	return consoleHandle;
}

COORD Console::GetConsoleWindowSize()
{
	CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &ScreenBufferInfo);
	COORD size;
	size.X = ScreenBufferInfo.srWindow.Right - ScreenBufferInfo.srWindow.Left + 1;
	size.Y = ScreenBufferInfo.srWindow.Bottom - ScreenBufferInfo.srWindow.Top + 1;
	return size;
}

bool Console::EnableVTMode()
{
	DWORD dwMode = 0;
	if (!GetConsoleMode(consoleHandle, &dwMode))
	{
		return false;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(consoleHandle, dwMode))
	{
		return false;
	}
	return true;
}

void Console::FixConsoleSize()
{
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLongPtr(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_VSCROLL & ~WS_HSCROLL);
}

bool Console::SetConsoleBuffer()
{
	CONSOLE_SCREEN_BUFFER_INFOEX bufferInfo;
	bufferInfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	auto fasd = GetConsoleScreenBufferInfoEx(consoleHandle, &bufferInfo);

	bufferInfo.dwMaximumWindowSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
	bufferInfo.dwSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
	bufferInfo.srWindow = { 0, 0, CONSOLE_HEIGHT - 2, CONSOLE_WIDTH - 2 };
	
	// Change the console window size:
	if (!SetConsoleScreenBufferInfoEx(consoleHandle, &bufferInfo))
	{
		return false;
	}
}
