#pragma once
#include <windows.h>
#include <vector>

enum Direction;

class Snake
{
public:
	Snake(int screenWidth, int screenHeight);

	~Snake();

	void MoveSnake();
	void AddTail();
private:
	int screenWidth;
	int screenHeight;
	Direction direction;
	std::vector<COORD> snake;
};

