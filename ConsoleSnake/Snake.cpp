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

bool Snake::MakeMove()
{
	Coordinates headPos = Coordinates(snake.front());
	Coordinates tailPos = Coordinates(snake.back());

	for (size_t i = 0; i < snake.size() - 1; i++)
	{
		std::iter_swap(snake.rbegin() + i, snake.rbegin() + i + 1);
	}
	SetHeadLocation(headPos);
	if (!CanMove(headPos))
		return false;
	console->DrawChar(snake.front());
	console->RemoveChar(tailPos);
	return true;
}

void Snake::SetDirection(Direction direction)
{
	this->direction = direction;
}

const Direction& Snake::GetDirection()
{
	return direction;
}

bool Snake::ChangeDirection(KEY_EVENT_RECORD ker)
{
	if (!ker.bKeyDown)
	{
		switch (ker.wVirtualKeyCode)
		{
		case VK_LEFT:
			if (direction != Direction::RIGHT)
			{
				direction = Direction::LEFT;
				return true;
			}
			break;
		case VK_RIGHT:
			if (direction != Direction::LEFT)
			{
				direction = Direction::RIGHT;
				return true;
			}
			break;
		case VK_UP:
			if (direction != Direction::DOWN)
			{
				direction = Direction::UP;
				return true;
			}
			break;
		case VK_DOWN:
			if (direction != Direction::UP)
			{
				direction = Direction::DOWN;
				return true;
			}
			break;
		default:
			return false;
		}
	}
	return false;
}

void Snake::SetHeadLocation(Coordinates &headPos)
{
	switch (direction)
	{
	case Direction::UP:
		headPos.Y = headPos.Y - 1 < 1 ? screenHeight : headPos.Y - 1;
		break;
	case Direction::DOWN:
		headPos.Y = headPos.Y + 1 > screenHeight ? 1 : headPos.Y + 1;
		break;
	case Direction::LEFT:
		headPos.X = headPos.X - 1 < 1 ? screenWidth : headPos.X - 1;
		break;
	case Direction::RIGHT:
		headPos.X = headPos.X + 1  > screenWidth ? 1 : headPos.X + 1;
		break;
	}
	snake[0].X = headPos.X;
	snake[0].Y = headPos.Y;
}

bool Snake::CanMove(Coordinates& newPos)
{
	wchar_t ch = console->GetCharacterOnPositon(newPos);
	if (ch == '@')
	{
		return false;
	}
	return true;
}

void Snake::AddTail()
{
	auto coords = Coordinates{ 1, 1 };
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
		coords.X = xCoord > 0 ? xCoord : screenWidth;
		coords.Y = yCoord > 0 ? yCoord : screenHeight;
	}
	snake.push_back(coords);
}
