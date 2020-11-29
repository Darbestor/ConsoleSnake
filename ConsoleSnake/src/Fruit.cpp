#include "Fruit.h"
#include "Console.h"
#include <cstdlib>
#include <unordered_set>
#include "GameCharacter.h"

void Fruit::SpawnFruit()
{
	auto size = Console::GetConsoleWindowSize();
	Coordinates fruitCoord;
	wchar_t c = 0;
	do
	{
		fruitCoord.X = std::rand() % size.X + 1;
		fruitCoord.Y = std::rand() % size.Y + 1;
		c = Console::GetCharacterOnPositon(fruitCoord);
	} while (c != ' ');
	Console::DrawChar(fruitCoord, GameCharacter::FRUIT);
}
