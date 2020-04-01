#include <iostream>
#include <memory>
#include <ctime>
#include "Revolver.h"

Revolver::Revolver(int MaxMag, int Bullet, int InitShuffle)
	: MaxMagazine(MaxMag)
	, InsertedBullet(Bullet)
	, NowBullet(0)
{
	NowMagazine.reset(new bool[MaxMagazine]);

	// 배열을 초기화
	for (int i = 0; i < MaxMagazine; ++i)
	{
		NowMagazine[i] = false;
	}

	// 배열에 총알 개수 만큼 true를 입력
	for (int i = 0; i < InsertedBullet; ++i)
	{
		NowMagazine[i] = true;
	}
	
	// InitShuffle수 만큼 배열요소를 스왑
	srand(static_cast<unsigned int>(time(nullptr)));
	for (int i = 0; i < InitShuffle; ++i)
	{
		int RandAddress1 = rand() % MaxMagazine;
		int RandAddress2 = rand() % MaxMagazine;

		if (NowMagazine[RandAddress1] != NowMagazine[RandAddress2])
		{
			int Temp = NowMagazine[RandAddress1];
			NowMagazine[RandAddress1] = NowMagazine[RandAddress2];
			NowMagazine[RandAddress2] = Temp;
		}
		else
		{
			i--;
			continue;
		}
	}
}

// 대기중인 총알의 위치를 변경 및 배열의 상태 반환
bool Revolver::Bang()
{
	NowBullet++;
	if (MaxMagazine <= NowBullet)
	{
		NowBullet = 0;
	}

	if (NowMagazine[NowBullet] == true)
	{
		NowMagazine[NowBullet] = false;
		return true;
	}
	else
	{
		return false;
	}
}

// 실린더가 비어있는지 검사
bool Revolver::IsEmpty() const
{
	for (int i = 0; i < MaxMagazine; ++i)
	{
		if (NowMagazine[i] == true)
		{
			return false;
		}
	}

	return true;
}

// NowBullet의 위치를 무작위로 변경
void Revolver::CylinderRotation()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	NowBullet = rand() % MaxMagazine;
}
