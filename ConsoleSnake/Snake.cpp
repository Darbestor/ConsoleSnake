#include "Snake.h"
#include <iostream>
#include "Console.h"

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
	Coordinates *prevElement = &(snake.back());
	Coordinates *currentElement = nullptr;
	MoveHead();

	for (auto it = std::begin(snake) + 1; it != std::end(snake); ++it) {
		currentElement = &(*it);
		*it = *prevElement;
		prevElement = currentElement;
		DrawChar();
	}
	delete prevElement;
}

void Snake::DrawChar()
{
	std::cout << "@";
}

void Snake::MoveHead()
{
	Coordinates& lastPositionPtr = snake.back();
	Coordinates* newHeadLocation = new Coordinates{ lastPositionPtr.X, lastPositionPtr.Y};
	switch (direction)
	{
	case Direction::UP:
		newHeadLocation->Y = lastPositionPtr.Y + 1 > screenHeight ? 0 : lastPositionPtr.Y + 1;
		break;
	case Direction::DOWN:
		newHeadLocation->Y = lastPositionPtr.Y - 1 < 0 ? screenHeight : lastPositionPtr.Y - 1;
		break;
	case Direction::LEFT:
		newHeadLocation->X = lastPositionPtr.X - 1 < 0 ? screenWidth : lastPositionPtr.X - 1;
		break;
	case Direction::RIGHT:
		newHeadLocation->X = lastPositionPtr.X + 1  > screenWidth ? 0 : lastPositionPtr.X + 1;
		break;
	}
	snake[snake.size() - 1] = *newHeadLocation;
	DrawChar();
}

void Snake::AddTail()
{
	auto coords = Coordinates{ 0, 0 };
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