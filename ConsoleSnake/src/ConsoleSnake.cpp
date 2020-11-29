// System headers
#include <iostream>
#include <string>

// Standard library C-style
#include <wchar.h>
#include <ctime>
#include "Console.h"
#include "Snake.h"

// Escape sequences
#define ESC "\x1b"
#define CSI "\x1b["

#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 40

VOID HandleInput();

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
	HANDLE inHandle = console.GetConsoleInputHandle();
	auto snake = Snake(&console);

	// Enter alternate buffer
	std::cout << CSI "?1h";
	int i = 0;
	int keyCode;
	while (true)
	{
		if (console.CheckKeyReleased(&keyCode))
		{

		}
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