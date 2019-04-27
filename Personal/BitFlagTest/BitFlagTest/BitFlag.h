#pragma once

class BitFlag
{
public:
	BitFlag();

	void SetBitFlag(const int Index, const bool Value);
	bool GetBitFlag(const int Index) const;
	inline int FindAddress(const int Index) const;
	int GetDirectData() const;

private:
	int Data;
};
