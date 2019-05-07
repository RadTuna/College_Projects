#include <iostream>
#include "Revolver.h"
#include "RouletteManager.h"

int main()
{
	Revolver Pistol(8, 3);

	RouletteManager::SetMode(true, false, true);
	RouletteManager::PlayRoulette(Pistol, 6);

	return 0;
}