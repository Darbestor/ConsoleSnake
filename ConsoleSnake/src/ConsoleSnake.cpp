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

VOID HandleInput(int &keyCode, Snake &snake)
{
	switch (keyCode)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_UP:
	case VK_DOWN:
		snake.ChangeDirection(keyCode);
	}
}

int __cdecl wmain(int argc, WCHAR* argv[])
{
	int consoleWidth = 0;
	int consoleHeight = 0;
	if (argc == 3)
	{
		try {
			int width = std::stoi(argv[1]);
			int height = std::stoi(argv[2]);
			int consoleWidth = width;
			int consoleHeight = height;
		}
		catch (std::exception const& e) {
			std::cout << "Argument type invalid\n";
			return -1;
		}
	}
	bool fSuccess = Console::SetupConsole(consoleWidth, consoleHeight);
	if (!fSuccess)
	{
		printf("Unable to enter VT processing mode. Quitting.\n");
		return -1;
	}
	HANDLE handle = Console::GetConsoleHandle();
	HANDLE inHandle = Console::GetConsoleInputHandle();
	auto screenSize = Console::GetConsoleWindowSize();
	auto snake = Snake(screenSize);

	// Enter alternate buffer
	std::cout << CSI "?1h";
	int i = 0;
	int keyCode;
	while (true)
	{
		if (Console::CheckKeyReleased(&keyCode))
		{
			HandleInput(keyCode, snake);
		}
		if (!snake.MakeMove())
		{
			std::cout << CSI "2J";
			std::cout << "You lost!";
			break;
		}

		auto t = clock();
		while (difftime(clock(), t) < 50) {}
		i++;
		if (i % 10 == 0)
		{
			snake.AddTail();
		}
	}

	// Exit the alternate buffer
	printf(CSI "?1049l");

}