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

	// �迭�� �ʱ�ȭ
	for (int i = 0; i < MaxMagazine; ++i)
	{
		NowMagazine[i] = false;
	}

	// �迭�� �Ѿ� ���� ��ŭ true�� �Է�
	for (int i = 0; i < InsertedBullet; ++i)
	{
		NowMagazine[i] = true;
	}
	
	// InitShuffle�� ��ŭ �迭��Ҹ� ����
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

// ������� �Ѿ��� ��ġ�� ���� �� �迭�� ���� ��ȯ
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

// �Ǹ����� ����ִ��� �˻�
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

// NowBullet�� ��ġ�� �������� ����
void Revolver::CylinderRotation()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	NowBullet = rand() % MaxMagazine;
}
