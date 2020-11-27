#pragma once
#include <windows.h>

class Console
{
public:
	Console(short width, short height);

	bool SetupConsole();
	HANDLE GetConsoleHandle();
	COORD GetConsoleWindowSize();
	COORD GetConsoleCursorPosition();

private:
	bool EnableVTMode();
	void FixConsoleSize();
	bool SetConsoleBuffer();

	short windowWidth;
	short windowHeight;
	HANDLE consoleHandle;
};
