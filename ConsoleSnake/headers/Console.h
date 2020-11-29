#pragma once
#include <windows.h>
#include "Coordinates.h"

class Console
{
public:

	static bool SetupConsole(short width = 0, short height = 0);
	static const HANDLE GetConsoleHandle();
	static const HANDLE GetConsoleInputHandle();
	static Coordinates GetConsoleWindowSize();
	static Coordinates GetConsoleCursorPosition();
	static wchar_t GetCharacterOnPositon(Coordinates &position);
	static void DrawChar(const Coordinates &cursorPosition, const wchar_t& symbol);
	static void RemoveChar(const Coordinates& cursorPosition);
	static bool CheckKeyReleased(int *keyCode);

private:
	// Disallow creating instance
	Console() {}

	static bool EnableVTMode();
	static void FixConsoleSize();
	static bool SetConsoleBuffer();

	static short windowWidth;
	static short windowHeight;
	static HANDLE handle;
	static HANDLE inHandle;
};

