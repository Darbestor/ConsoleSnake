#pragma once
#include <windows.h>
#include "Coordinates.h"

class Console
{
public:
	Console(short width, short height);

	bool SetupConsole();
	HANDLE GetConsoleHandle();
	COORD GetConsoleWindowSize();
	COORD GetConsoleCursorPosition();
	void DrawChar(const Coordinates &cursorPosition);

private:
	bool EnableVTMode();
	void FixConsoleSize();
	bool SetConsoleBuffer();

	short windowWidth;
	short windowHeight;
	HANDLE consoleHandle;
};

