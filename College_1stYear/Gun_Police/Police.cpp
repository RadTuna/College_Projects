#include "Police.h"
#include <iostream>

Police::Police(int inBullet, int inHandcuffs)
	: handcuffs(inHandcuffs)
	, pistol(nullptr)
{
	pistol = new Gun(inBullet);
}

Police::Police(const Police& other)
	: handcuffs(other.handcuffs)
	, pistol(nullptr)
{
	pistol = new Gun(other.pistol->GetBullet());
}

Police::~Police()
{
	if (pistol != nullptr)
	{
		delete pistol;
		pistol = nullptr;
	}
}

Police& Police::operator=(const Police& other)
{
	if (pistol != nullptr)
	{
		delete pistol;
		pistol = nullptr;
	}
	handcuffs = other.handcuffs;
	pistol = new Gun(other.pistol->GetBullet());
	return *this;
}

void Police::PutHandcuff()
{
	std::cout << "수갑을 채우다" << std::endl;
	--handcuffs;
}

void Police::ShowHandcuff()
{
	std::cout << "남은 수갑의 개수 : " << handcuffs << std::endl;
}

void Police::Shot()
{
	pistol->Shot();
}

void Police::ShowBullet()
{
	pistol->ShowBullet();
}
