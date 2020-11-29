#pragma once

class Fruit
{
public:
	Fruit();
	void SpawnFruit();
	void SetSpawned(bool isSpawned);
	const bool &GetSpawned();
private:
	bool isSpawned;
};

