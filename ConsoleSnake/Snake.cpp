#include "Snake.h"
#include "Direction.h"

Snake::Snake(int screenWidth, int screenHeight) :
	screenWidth(screenWidth - 1),
	screenHeight(screenHeight - 1),
	direction(Direction::RIGHT)
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
		int xCoord;
		int yCoord;
		auto lastElement = snake.back();
		if (snake.size() == 1)
		{
			switch (direction)
			{
			case Direction::UP:
				yCoord = lastElement.Y + 1;
				xCoord = lastElement.X;
				break;
			case Direction::DOWN:
				yCoord = lastElement.Y - 1;
				xCoord = lastElement.X;
				break;
			case Direction::LEFT:
				xCoord = lastElement.X + 1;
				yCoord = lastElement.Y;
				break;
			case Direction::RIGHT:
				xCoord = lastElement.X - 1;
				yCoord = lastElement.Y;
				break;
			}
		}
		else
		{
			auto prevElement = snake[snake.size() - 2];
			xCoord = lastElement.X + (lastElement.X - prevElement.X);
			yCoord = lastElement.Y + (lastElement.Y - prevElement.Y);
		}
		coords.X = xCoord > -1 ? xCoord : screenWidth;
		coords.Y = yCoord > -1 ? yCoord : screenHeight;
	}
	snake.push_back(coords);

}
