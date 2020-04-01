#include "CSet.h"
#include <limits>
#include <queue>
#include <cmath>
#include <iostream>

CSet::CSet()
	: Num(0)
{
	Empty();
}

CSet::~CSet()
{
}

void CSet::Empty()
{
	for (int i = 0; i < MAX; ++i)
	{
		Elems[i] = std::numeric_limits<int>::max();
	}
	Num = 0;
}

bool CSet::Member(int e)
{
	for (int i = 0; i < Num; ++i)
	{
		if (Elems[i] == e)
		{
			return true;
		}
	}

	return false;
}

bool CSet::Insert(int e)
{
	if (Num >= MAX)
	{
		return false;
	}

	if (Member(e) == true)
	{
		return false;
	}

	Elems[Num] = e;
	Num++;

	return true;
}

void CSet::Remove(int e)
{
	for (int i = 0; i < MAX; ++i)
	{
		if (Elems[i] == e)
		{
			Elems[i] = std::numeric_limits<int>::max();
			SortArr(Elems, MAX, 100);
			Num--;
			return;
		}
	}
}

bool CSet::Equal(CSet * s)
{
	SortArr(Elems, MAX, 100);
	SortArr(s->Elems, MAX, 100);

	for (int i = 0; i < MAX; ++i)
	{
		if (Elems[i] != s->Elems[i])
		{
			return false;
		}
	}

	return true;
}

bool CSet::Union(CSet* inSet, CSet * outSet)
{
	if (Num + inSet->Num > MAX)
	{
		return false;
	}

	outSet->Empty();

	for (int i = 0; i < Num; ++i)
	{
		outSet->Insert(Elems[i]);
	}
	for (int i = 0; i < inSet->Num; ++i)
	{
		outSet->Insert(inSet->Elems[i]);
	}

	return true;
}

bool CSet::Intersection(CSet * inSet, CSet * outSet)
{
	outSet->Empty();

	SortArr(Elems, MAX, 100);
	SortArr(inSet->Elems, MAX, 100);

	for (int i = 0; i < std::min(Num, inSet->Num); ++i)
	{
		if (Elems[i] == inSet->Elems[i])
		{
			outSet->Insert(Elems[i]);
		}
	}

	return true;
}

void CSet::Print()
{
	std::cout << "{";
	for (int i = 0; i < Num; ++i)
	{
		std::cout << Elems[i];
		if (i + 1 < Num)
		{
			std::cout << ", ";
		}
	}
	std::cout << "}";
	std::cout << std::endl;
}

bool CSet::operator[](int Index)
{
	return Member(Index);
}

bool CSet::operator()(int e)
{
	return Insert(e);
}

void CSet::operator-(int e)
{
	Remove(e);
}

CSet CSet::operator*(CSet& other)
{
	CSet OutSet;
	Intersection(&other, &OutSet);
	return OutSet;
}

bool CSet::operator==(CSet & other)
{
	return Equal(&other);
}

CSet CSet::operator+(CSet & other)
{
	CSet OutSet;
	Union(&other, &OutSet);
	return OutSet;
}

void CSet::SortArr(int* keyArr, int keyArrLen, int maxKey)
{
	enum { BUCKET_SIZE = 10 };

	std::queue<int> bucketList[BUCKET_SIZE];

	int PosNumCount = 1;
	for (int i = 10;; i *= 10, ++PosNumCount)
	{
		if (maxKey % i >= maxKey)
		{
			break;
		}
	}

	for (int i = 1; i <= PosNumCount; ++i)
	{
		for (int j = 0; j < keyArrLen; ++j)
		{
			int powNum = static_cast<int>(std::pow(10, i));
			bucketList[(keyArr[j] % powNum) / (powNum / 10)].push(keyArr[j]);
		}

		int keyArrCount = 0;
		for (int j = 0; j < BUCKET_SIZE; ++j)
		{
			if (bucketList[j].empty() == true)
			{
				continue;
			}

			for (; bucketList[j].empty() == false;)
			{
				keyArr[keyArrCount] = bucketList[j].front();
				bucketList[j].pop();
				++keyArrCount;
			}
		}
	}
}

