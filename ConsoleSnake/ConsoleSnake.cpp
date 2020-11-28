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

void PrintVerticalBorder()
{
	printf(ESC "(0"); // Enter Line drawing mode
	printf(CSI "104;93m"); // bright yellow on bright blue
	printf("x"); // in line drawing mode, \x78 -> \u2502 "Vertical Bar"
	printf(CSI "0m"); // restore color
	printf(ESC "(B"); // exit line drawing mode
}

//static struct DirectionKeys
//{
//	static CONST WCHAR UP{ L'sadfsd' };
//	static constexpr WCHAR DOWN[] = CSI "B";
//	static constexpr CHAR LEFT[] = CSI "D";
//	static constexpr char RIGHT[] = CSI "C";
//};

void PrintHorizontalBorder(COORD const size, bool fIsTop)
{
	printf(ESC "(0"); // Enter Line drawing mode
	printf(CSI "104;93m"); // Make the border bright yellow on bright blue
	printf(fIsTop ? "l" : "m"); // print left corner 

	for (int i = 1; i < size.X - 1; i++)
		printf("q"); // in line drawing mode, \x71 -> \u2500 "HORIZONTAL SCAN LINE-5"

	printf(fIsTop ? "k" : "j"); // print right corner
	printf(CSI "0m");
	printf(ESC "(B"); // exit line drawing mode
}

void PrintStatusLine(const char* const pszMessage, COORD const size)
{
	printf(CSI "%d;1H", size.Y);
	printf(CSI "K"); // clear the line
	printf(pszMessage);
}


bool ChangeDirection(KEY_EVENT_RECORD ker, Snake &snake)
{
	if (!ker.bKeyDown)
	{
		auto currentDirection = snake.GetDirection();
		switch (ker.wVirtualKeyCode)
		{
		case VK_LEFT:
			if (currentDirection != Direction::RIGHT)
			{
				snake.SetDirection(Direction::LEFT);
				return true;
			}
			break;
		case VK_RIGHT:
			if (currentDirection != Direction::LEFT)
			{
				snake.SetDirection(Direction::RIGHT);
				return true;
			}
			break;
		case VK_UP:
			if (currentDirection != Direction::DOWN)
			{
				snake.SetDirection(Direction::UP);
				return true;
			}
			break;
		case VK_DOWN:
			if (currentDirection != Direction::UP)
			{
				snake.SetDirection(Direction::DOWN);
				return true;
			}
			break;
		default:
			return false;
		}
	}
	return false;
}

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
				if (ChangeDirection(irInBuf[i].Event.KeyEvent, snake))
					return;
				break;
			}
		}
	}
}

int __cdecl wmain(int argc, WCHAR* argv[])
{
	argc; // unused
	argv; // unused
	//First, enable VT mode

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
	//// Enter the alternate buffer
	//printf(CSI "?1049h");

	//// Clear screen, tab stops, set, stop at columns 16, 32
	//printf(CSI "1;1H");
	//printf(CSI "2J"); // Clear screen

	//int iNumTabStops = 4; // (0, 20, 40, width)
	//printf(CSI "3g"); // clear all tab stops
	//printf(CSI "1;20H"); // Move to column 20
	//printf(ESC "H"); // set a tab stop

	//printf(CSI "1;40H"); // Move to column 40
	//printf(ESC "H"); // set a tab stop

	//// Set scrolling margins to 3, h-2
	//printf(CSI "3;%dr", size.Y - 2);
	//int iNumLines = size.Y - 4;

	//printf(CSI "1;1H");
	//printf(CSI "102;30m");
	//printf("Windows 10 Anniversary Update - VT Example");
	//printf(CSI "0m");

	//// Print a top border - Yellow
	//printf(CSI "2;1H");
	//PrintHorizontalBorder(size, true);

	//// // Print a bottom border
	//printf(CSI "%d;1H", size.Y - 1);
	//PrintHorizontalBorder(size, false);

	//wchar_t wch;

	//// draw columns
	//printf(CSI "3;1H");
	//int line = 0;
	//for (line = 0; line < iNumLines * iNumTabStops; line++)
	//{
	//	PrintVerticalBorder();
	//	if (line + 1 != iNumLines * iNumTabStops) // don't advance to next line if this is the last line
	//		printf("\t"); // advance to next tab stop

	//}

	//PrintStatusLine("Press any key to see text printed between tab stops.", size);
	//wch = _getwch();

	//// Fill columns with output
	//printf(CSI "3;1H");
	//for (line = 0; line < iNumLines; line++)
	//{
	//	int tab = 0;
	//	for (tab = 0; tab < iNumTabStops - 1; tab++)
	//	{
	//		PrintVerticalBorder();
	//		printf("line=%d", line);
	//		printf("\t"); // advance to next tab stop
	//	}
	//	PrintVerticalBorder();// print border at right side
	//	if (line + 1 != iNumLines)
	//		printf("\t"); // advance to next tab stop, (on the next line)
	//}

	//PrintStatusLine("Press any key to demonstrate scroll margins", size);
	//wch = _getwch();

	//printf(CSI "3;1H");
	//for (line = 0; line < iNumLines * 2; line++)
	//{
	//	printf(CSI "K"); // clear the line
	//	int tab = 0;
	//	for (tab = 0; tab < iNumTabStops - 1; tab++)
	//	{
	//		PrintVerticalBorder();
	//		printf("line=%d", line);
	//		printf("\t"); // advance to next tab stop
	//	}
	//	PrintVerticalBorder(); // print border at right side
	//	if (line + 1 != iNumLines * 2)
	//	{
	//		printf("\n"); //Advance to next line. If we're at the bottom of the margins, the text will scroll.
	//		printf("\r"); //return to first col in buffer
	//	}
	//}

	//PrintStatusLine("Press any key to exit", size);
	//wch = _getwch();

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