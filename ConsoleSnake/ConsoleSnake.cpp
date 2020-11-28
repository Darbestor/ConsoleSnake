// System headers
#include <iostream>
#include <string>

// Standard library C-style
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include "Console.h"
#include "Snake.h"
#include <algorithm>

#define ESC "\x1b"
#define CSI "\x1b["
#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 40

void HandleInput(HANDLE& handle, Snake& snake)
{
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
				if (snake.ChangeDirection(irInBuf[i].Event.KeyEvent))
					return;
				break;
			}
		}
	}
}

int __cdecl wmain(int argc, WCHAR* argv[])
{
	int consoleWidth = CONSOLE_WIDTH;
	int consoleHeight = CONSOLE_HEIGHT;
	if (argc == 3)
	{
		try {
			int width = std::stoi(argv[1]);
			int height = std::stoi(argv[2]);
			consoleWidth = width;
			consoleHeight = height;
		}
		catch (std::exception const& e) {
			std::cout << "Argument type invalid\n";
			return -1;
		}
	}
	auto console = Console(CONSOLE_WIDTH, CONSOLE_HEIGHT);
	bool fSuccess = console.SetupConsole();
	if (!fSuccess)
	{
		printf("Unable to enter VT processing mode. Quitting.\n");
		return -1;
	}
	HANDLE handle = console.GetConsoleHandle();
	if (handle == INVALID_HANDLE_VALUE)
	{
		printf("Couldn't get the console output handle. Quitting.\n");
		return -1;
	}
	HANDLE inHandle = console.GetConsoleInputHandle();
	if (handle == INVALID_HANDLE_VALUE)
	{
		printf("Couldn't get the console output handle. Quitting.\n");
		return -1;
	}

	auto size = console.GetConsoleWindowSize();
	auto snake = Snake(&console);

	std::cout << CSI "?1h";
	int i = 0;
	while (true)
	{
		HandleInput(inHandle, snake);
		if (!snake.MakeMove())
		{
			std::cout << "You lost!";
			break;
		}

		auto t = clock();
		while (difftime(clock(), t) < 250) {}
		//std::cout << CSI "2J";
		i++;
		if (i % 10 == 0)
		{
			snake.AddTail();
		}
	}

	// Exit the alternate buffer
	printf(CSI "?1049l");

}