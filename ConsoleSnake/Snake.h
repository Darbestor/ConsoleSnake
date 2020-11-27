#pragma once
#include <vector>
#include "Coordinates.h"

enum Direction;

class Snake
{
public:
	Snake(int screenWidth, int screenHeight);

	~Snake();

	void MoveSnake();
	void MoveHead();
	void AddTail();
private:
	int screenWidth;
	int screenHeight;
	Direction direction;
	std::vector<Coordinates> snake;
};

