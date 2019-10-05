#include "PlayerBase.h"

PlayerBase::PlayerBase()
{
	CurrentNumber = 0;
}

bool PlayerBase::CheckNumber(int InNum) const
{
	// 3자리의 숫자가 아님.
	if (InNum / 100 >= 10 || InNum / 100 < 1)
	{
		return false;
	}

	int FirstNum = InNum / 100;
	int SecondNum = (InNum / 10) % 10;
	int LastNum = InNum % 10;

	// 숫자에 중복이 있는지 검사.
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
