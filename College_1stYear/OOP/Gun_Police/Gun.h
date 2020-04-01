#pragma once
class Gun
{
public:

	Gun(int inBullet);
	
	void Shot();
	void ShowBullet();

private:

	int bullet;

public:

	inline int GetBullet() const { return bullet; }

};

