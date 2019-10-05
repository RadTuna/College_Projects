#pragma once

class PlayerBase
{
public:

	PlayerBase();
	~PlayerBase() = default;

	// true : 중복 검사 통과 / false : 중복 검사 실패
	// 오직 3자리의 입력을 받음. 그렇지 않을 경우 false 반환.
	bool CheckNumber(int InNum) const;

	virtual void InputNumber() = 0;

	void SetCurrentNumber(int InNum);

private:

	int CurrentNumber;

public:

	inline int GetCurrentNumber() const { return CurrentNumber; }

};

