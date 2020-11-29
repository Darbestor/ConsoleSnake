#pragma once
#include <vector>
#include "Coordinates.h"
#include "Direction.h"
#include <windows.h>

class Console;

class Snake
{
public:
	Snake(Coordinates &screenSize);

	~Snake();

	void AddTail();
	bool MakeMove();
	void SetDirection(Direction direction);
	const Direction& GetDirection();
	void ChangeDirection(int &keyCode);

private:
	int screenWidth;
	int screenHeight;
	Direction direction;
	std::vector<Coordinates> snake;
	
	void SetHeadLocation(Coordinates &headPos);
	bool ManageMovement(Coordinates &headPos);
};

