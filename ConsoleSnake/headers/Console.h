#pragma once
#include <windows.h>
#include "Coordinates.h"

class Console
{
public:
	Console(short width, short height);

	bool SetupConsole();
	HANDLE GetConsoleHandle();
	HANDLE GetConsoleInputHandle();
	COORD GetConsoleWindowSize();
	COORD GetConsoleCursorPosition();
	wchar_t GetCharacterOnPositon(Coordinates &position);
	void DrawChar(const Coordinates &cursorPosition);
	void RemoveChar(const Coordinates& cursorPosition);

private:
	bool EnableVTMode();
	void FixConsoleSize();
	bool SetConsoleBuffer();

	short windowWidth;
	short windowHeight;
	HANDLE handle;
	HANDLE inHandle;
};

