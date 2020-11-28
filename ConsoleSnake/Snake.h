#pragma once
#include <vector>
#include "Coordinates.h"
#include "Direction.h"

class Snake
{
public:
	Snake(int screenWidth, int screenHeight);

	~Snake();

	void AddTail();
	void MoveSnake();
private:
	int screenWidth;
	int screenHeight;
	Direction direction;
	std::vector<Coordinates> snake;
	
	void MoveHead();
	void DrawChar();
};

