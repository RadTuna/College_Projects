#pragma once

#include "Gun.h"

class Police
{
public:

	Police(int inBullet, int inHandcuffs);
	Police(const Police& other);
	~Police();

	Police& operator=(const Police& other);

	void PutHandcuff();
	void ShowHandcuff();
	void Shot();
	void ShowBullet();

private:

	int handcuffs;
	Gun* pistol;

};

