#pragma once
#include <windows.h>
#include "Coordinates.h"

class Console
{
public:
	Console(short width, short height);

	bool SetupConsole();
	const HANDLE GetConsoleHandle() const;
	const HANDLE GetConsoleInputHandle() const;
	Coordinates GetConsoleWindowSize() const;
	Coordinates GetConsoleCursorPosition() const;
	wchar_t GetCharacterOnPositon(Coordinates &position);
	void DrawChar(const Coordinates &cursorPosition);
	void RemoveChar(const Coordinates& cursorPosition);
	bool CheckKeyReleased(int *keyCode);

private:
	bool EnableVTMode();
	void FixConsoleSize();
	bool SetConsoleBuffer();

	short windowWidth;
	short windowHeight;
	HANDLE handle;
	HANDLE inHandle;
};

