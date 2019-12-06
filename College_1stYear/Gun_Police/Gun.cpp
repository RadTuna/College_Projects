#include "Gun.h"
#include <iostream>

Gun::Gun(int inBullet)
	: bullet(inBullet)
{
}

void Gun::Shot()
{
	std::cout << "ÃÑÀ» ½î´Ù" << std::endl;
	--bullet;
}

void Gun::ShowBullet()
{
	std::cout << "³²Àº ÃÑ¾ËÀÇ °³¼ö : " << bullet << std::endl;
}
