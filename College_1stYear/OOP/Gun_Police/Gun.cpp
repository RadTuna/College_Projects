#include "Gun.h"
#include <iostream>

Gun::Gun(int inBullet)
	: bullet(inBullet)
{
}

void Gun::Shot()
{
	std::cout << "���� ���" << std::endl;
	--bullet;
}

void Gun::ShowBullet()
{
	std::cout << "���� �Ѿ��� ���� : " << bullet << std::endl;
}
