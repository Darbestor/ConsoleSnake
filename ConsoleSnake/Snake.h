#pragma once
#include <vector>
#include "Coordinates.h"
#include "Direction.h"
#include <windows.h>

class Console;

class Snake
{
public:
	Snake(Console* console);

	~Snake();

	void AddTail();
	bool MakeMove();
	void SetDirection(Direction direction);
	const Direction& GetDirection();
	bool ChangeDirection(KEY_EVENT_RECORD ker);

private:
	int screenWidth;
	int screenHeight;
	Direction direction;
	std::vector<Coordinates> snake;
	Console *console;
	
	void SetHeadLocation(Coordinates &headPos);
	bool CanMove(Coordinates &headPos);
};

