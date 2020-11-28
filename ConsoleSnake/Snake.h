#pragma once
#include <vector>
#include "Coordinates.h"
#include "Direction.h"

class Console;

class Snake
{
public:
	Snake(Console* console);

	~Snake();

	void AddTail();
	void MoveSnake();
	void SetDirection(Direction direction);
private:
	int screenWidth;
	int screenHeight;
	Direction direction;
	std::vector<Coordinates> snake;
	Console *console;
	
	void MoveHead(Coordinates &headPos);
};

