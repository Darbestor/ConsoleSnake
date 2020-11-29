//
// Copyright (C) Microsoft. All rights reserved.
//
#include "Console.h"
#include "Snake.h"
#define DEFINE_CONSOLEV2_PROPERTIES
#include <iostream>
#include <string>

#define ESC "\x1b"
#define CSI "\x1b["
#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 40

Console::Console(short width, short height)
{
	windowHeight = height;
	windowWidth = width;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle == INVALID_HANDLE_VALUE)
	{
		throw std::runtime_error("Fail to get Output handle");
	}
	inHandle = GetStdHandle(STD_INPUT_HANDLE);
	if (inHandle == INVALID_HANDLE_VALUE)
	{
		throw std::runtime_error("Fail to get Input handle");
	}
}

bool Console::SetupConsole()
{
	bool fSuccess = EnableVTMode();
	FixConsoleSize();
	fSuccess = SetConsoleBuffer();
	return fSuccess;
}

const HANDLE Console::GetConsoleHandle() const
{
	return handle;
}

const HANDLE Console::GetConsoleInputHandle() const
{
	return inHandle;
}

Coordinates Console::GetConsoleWindowSize() const
{
	CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
	GetConsoleScreenBufferInfo(handle, &ScreenBufferInfo);
	Coordinates size;
	size.X = ScreenBufferInfo.srWindow.Right - ScreenBufferInfo.srWindow.Left + 1;
	size.Y = ScreenBufferInfo.srWindow.Bottom - ScreenBufferInfo.srWindow.Top + 1;
	return size;
}

bool Console::EnableVTMode()
{
	DWORD dwMode = 0;
	if (!GetConsoleMode(handle, &dwMode))
	{
		return false;
	}
	DWORD fdwMode = 0;
	if (!GetConsoleMode(inHandle, &fdwMode))
	{
		return false;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING & ~DISABLE_NEWLINE_AUTO_RETURN;
	if (!SetConsoleMode(handle, dwMode))
	{
		return false;
	}

	fdwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_VIRTUAL_TERMINAL_INPUT & ~ENABLE_MOUSE_INPUT & ~ENABLE_INSERT_MODE & ~ENABLE_WINDOW_INPUT;
	if (!SetConsoleMode(inHandle, fdwMode))
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
	auto fasd = GetConsoleScreenBufferInfoEx(handle, &bufferInfo);

	bufferInfo.dwMaximumWindowSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
	bufferInfo.dwSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
	bufferInfo.srWindow = { 0, 0, CONSOLE_HEIGHT - 2, CONSOLE_WIDTH - 2 };
	
	// Change the console window size:
	if (!SetConsoleScreenBufferInfoEx(handle, &bufferInfo))
	{
		return false;
	}
}

Coordinates Console::GetConsoleCursorPosition() const
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	Coordinates size{ 0, 0 };
	if (GetConsoleScreenBufferInfo(handle, &cbsi))
	{
		size.X = cbsi.dwCursorPosition.X;
		size.Y = cbsi.dwCursorPosition.Y;
	}
	return size;
}


wchar_t Console::GetCharacterOnPositon(Coordinates& position)
{
	std::cout << CSI + std::to_string(position.Y) + ";" + std::to_string(position.X) + "H";
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(handle, &csbiInfo);
	COORD pos = csbiInfo.dwCursorPosition;; //set pos to current cursor location
	TCHAR strFromConsole[1];    //need space to only one char
	DWORD dwChars;
	ReadConsoleOutputCharacter(
		handle,
		strFromConsole, // Buffer where store symbols
		1, // Read 1 char to strFormConsole
		pos, // Read from current cursor position
		&dwChars); // How many symbols stored

	wchar_t c = strFromConsole[0];
	return c;
}

void Console::DrawChar(const Coordinates &cursorPosition)
{
	std::cout << CSI + std::to_string(cursorPosition.Y) + ";" + std::to_string(cursorPosition.X) + "H";
	std::cout << "@";
}

void Console::RemoveChar(const Coordinates& cursorPosition)
{
	std::cout << CSI + std::to_string(cursorPosition.Y) + ";" + std::to_string(cursorPosition.X) + "H";
	std::cout << CSI "1X";
}

bool Console::CheckKeyReleased(int* keyCode)
{
	if (keyCode == nullptr)
	{
		throw std::invalid_argument("keyCode is NULL");
	}
	DWORD numberOfEvents;
	DWORD cNumRead;
	PINPUT_RECORD irInBuf;
	GetNumberOfConsoleInputEvents(handle, &numberOfEvents);
	if (numberOfEvents > 0)
	{
		irInBuf = new INPUT_RECORD[numberOfEvents];
		ReadConsoleInput(
			handle,			// input buffer handle
			irInBuf,		// buffer to read into
			numberOfEvents,	// size of read buffer
			&cNumRead);		// number of records read

		// Dispatch the events to the appropriate handler.
		for (INT i = 0; i < cNumRead; i++)
		{

			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input
				if (!irInBuf[i].Event.KeyEvent.bKeyDown)
				{
					*keyCode = irInBuf[i].Event.KeyEvent.wVirtualKeyCode;
					return true;
				}
				break;
			}
		}
	}
	*keyCode = -1;
	return false;
}
