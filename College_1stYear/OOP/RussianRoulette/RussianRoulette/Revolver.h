#pragma once

class Revolver final
{
public:
	Revolver(int MaxMag = 6, int Bullet = 1, int InitShuffle = 10);

	inline int GetNowBullet() const;
	inline int GetMaxMagazine() const;
	inline int GetInsertedBullet() const;
	bool Bang();
	bool IsEmpty() const;
	void CylinderRotation();

private:
	const int MaxMagazine;
	std::unique_ptr<bool[]> NowMagazine;
	int InsertedBullet;
	int NowBullet;
};

int Revolver::GetNowBullet() const
{
	return NowBullet;
}

int Revolver::GetMaxMagazine() const
{
	return MaxMagazine;
}

int Revolver::GetInsertedBullet() const
{
	return InsertedBullet;
}