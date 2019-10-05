#include "PlayerBase.h"

PlayerBase::PlayerBase()
{
	CurrentNumber = 0;
}

bool PlayerBase::CheckNumber(int InNum) const
{
	// 3�ڸ��� ���ڰ� �ƴ�.
	if (InNum / 100 >= 10 || InNum / 100 < 1)
	{
		return false;
	}

	int FirstNum = InNum / 100;
	int SecondNum = (InNum / 10) % 10;
	int LastNum = InNum % 10;

	// ���ڿ� �ߺ��� �ִ��� �˻�.
	if (FirstNum == SecondNum || FirstNum == LastNum || SecondNum == LastNum)
	{
		return false;
	}

	return true;
}

void PlayerBase::SetCurrentNumber(int InNum)
{
	CurrentNumber = InNum;
}
