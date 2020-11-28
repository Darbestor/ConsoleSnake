#include "Snake.h"
#include <iostream>
#include "Console.h"

Snake::Snake(Console* console) :
	direction(Direction::RIGHT),
	console(console)
{
	auto size = console->GetConsoleWindowSize();
	screenHeight = size.Y - 1;
	screenWidth = size.X - 1;
	AddTail();
	AddTail();
}

Snake::~Snake()
{
}

void Snake::MoveSnake()
{
	Coordinates headPos = Coordinates(snake.front());
	Coordinates tailPos = Coordinates(snake.back());

	for (size_t i = 0; i < snake.size() - 1; i++)
	{
		std::iter_swap(snake.rbegin() + i, snake.rbegin() + i + 1);
	}
	MoveHead(headPos);
	console->DrawChar(snake.front());
	console->RemoveChar(tailPos);
}

void Snake::SetDirection(Direction direction)
{
	this->direction = direction;
}

void Snake::MoveHead(Coordinates &headPos)
{
	switch (direction)
	{
	case Direction::UP:
		headPos.Y = headPos.Y - 1 < 0 ? screenHeight : headPos.Y - 1;
		break;
	case Direction::DOWN:
		headPos.Y = headPos.Y + 1 > screenHeight ? 0 : headPos.Y + 1;
		break;
	case Direction::LEFT:
		headPos.X = headPos.X - 1 < 0 ? screenWidth : headPos.X - 1;
		break;
	case Direction::RIGHT:
		headPos.X = headPos.X + 1  > screenWidth ? 0 : headPos.X + 1;
		break;
	}
	snake[0].X = headPos.X;
	snake[0].Y = headPos.Y;
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
				yCoord = lastElement.Y - 1;
				xCoord = lastElement.X;
				break;
			case Direction::DOWN:
				yCoord = lastElement.Y + 1;
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
