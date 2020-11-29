// System headers
#include <iostream>
#include <string>

// Standard library C-style
#include <wchar.h>
#include <ctime>
#include "Console.h"
#include "Snake.h"
#include "Fruit.h"

// Escape sequences
#define ESC "\x1b"
#define CSI "\x1b["
#define OCS "\x1b]"

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
	if (argc == 2)
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
	auto fruit = Fruit();

	// Enter alternate buffer
	std::cout << CSI "?1h";
	// Clear screen
	std::cout << CSI "2J";
	// Disable cursor visibilty
	std::cout << CSI "?25l";
	// Set title 
	std::cout << OCS "2;Console Snake\x07";

	int i = 0;
	int keyCode;
	double timeDiff = 500;
	double timeToSpeedUp = 10000;
	auto initialTime = clock();
	while (true)
	{
		if (Console::CheckKeyReleased(&keyCode))
		{
			HandleInput(keyCode, snake);
		}
		if (!snake.MakeMove(fruit))
		{
			// Clear screen
			std::cout << CSI "2J";
			// Reset cursor position
			std::cout << CSI "1;1H";
			std::cout << "You lost!";
			break;
		}
		if (!fruit.GetSpawned())
		{
			fruit.SpawnFruit();
		}
		if (difftime(clock(), initialTime) > timeToSpeedUp && timeDiff > 1)
		{
			timeDiff /= 1.5;
			timeToSpeedUp += 10000;
			initialTime = clock();
		}

		auto t = clock();
		while (difftime(clock(), t) < timeDiff) {}
	}

	// Exit the alternate buffer
	printf(CSI "?1049l");

}