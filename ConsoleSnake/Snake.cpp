#include "Snake.h"
#include "Direction.h"

Snake::Snake(int screenWidth, int screenHeight) :
	screenWidth(screenWidth - 1),
	screenHeight(screenHeight - 1)
{
	AddTail();
}

Snake::~Snake()
{
}

void Snake::MoveSnake()
{
}

void Snake::AddTail()
{
	auto coords = COORD{ 0, 0 };
	if (!snake.empty())
	{
		if (snake.size() == 1)
		{

		}
		auto lastElement = snake.back();
		auto prevElement = snake[snake.size() - 2];
		auto xCoord = lastElement.X + (lastElement.X - prevElement.X);
		auto yCoord = lastElement.Y + (lastElement.Y - prevElement.Y);
		coords.X = xCoord > -1 ? xCoord : screenWidth;
		coords.Y = yCoord > -1 ? yCoord : screenWidth;
	}
	snake.push_back(coords);

}
