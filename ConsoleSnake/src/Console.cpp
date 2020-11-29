//
// Copyright (C) Microsoft. All rights reserved.
//
#include "Console.h"
#include "Snake.h"
#define DEFINE_CONSOLEV2_PROPERTIES
#define _O_U16TEXT  0x20000
#include <iostream>
#include <string>

#define ESC "\x1b"
#define CSI "\x1b["
// standard Console size
#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 40

short Console::windowWidth = CONSOLE_WIDTH;
short Console::windowHeight = CONSOLE_HEIGHT;
HANDLE Console::handle = nullptr;
HANDLE Console::inHandle = nullptr;

bool Console::SetupConsole(short width, short height)
{
	if (width != 0)
	{
		windowWidth = width;
	}
	if (height != 0)
	{
		windowHeight = height;
	}
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
	bool fSuccess = EnableVTMode();
	if (!fSuccess)
	{
		return fSuccess;
	}
	FixConsoleSize();
	fSuccess = SetConsoleBuffer();
	if (!SetConsoleOutputCP(CP_UTF8))
	{
		throw std::runtime_error("Fail to set unicode");
	}
	return fSuccess;
}

const HANDLE Console::GetConsoleHandle()
{
	return handle;
}

const HANDLE Console::GetConsoleInputHandle()
{
	return inHandle;
}

Coordinates Console::GetConsoleWindowSize()
{
	return {windowWidth, windowHeight};
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

	bufferInfo.dwMaximumWindowSize = { windowWidth, windowHeight };
	bufferInfo.dwSize = { windowWidth, windowHeight };
	bufferInfo.srWindow = { 0, 0, windowHeight - 2, windowWidth - 2 };
	
	// Change the console window size:
	if (!SetConsoleScreenBufferInfoEx(handle, &bufferInfo))
	{
		return false;
	}
}

Coordinates Console::GetConsoleCursorPosition()
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

void Console::DrawChar(const Coordinates &cursorPosition, const wchar_t &symbol)
{
	std::cout << CSI + std::to_string(cursorPosition.Y) + ";" + std::to_string(cursorPosition.X) + "H";
	std::wcout << symbol;
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
	GetNumberOfConsoleInputEvents(inHandle, &numberOfEvents);
	if (numberOfEvents > 0)
	{
		irInBuf = new INPUT_RECORD[numberOfEvents];
		ReadConsoleInputW(
			inHandle,			// input buffer handle
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
