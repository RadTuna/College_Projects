#include "BitFlag.h"
#include <iostream>

BitFlag::BitFlag() : Data(0) { };

int BitFlag::FindAddress(const int Index) const
{
	int Address = 1;
	for (int i = 0; i < Index; ++i)
	{
		Address *= 2;
	}

	return Address;
}

void BitFlag::SetBitFlag(const int Index, const bool Value)
{
	if (Index < 0 || Index > 31)
	{
		return;
	}

	int Address = FindAddress(Index);

	if (Value == true)
	{
		Data |= Address;
	}
	else
	{
		Data ^= Address;
	}
}

bool BitFlag::GetBitFlag(const int Index) const
{
	if (Index < 0 || Index > 31)
	{
		std::cout << "False Input Integer Outbound" << std::endl;
		return false;
	}

	int Address = FindAddress(Index);

	return (Data & Address);
}

int BitFlag::GetDirectData() const
{
	return Data;
}