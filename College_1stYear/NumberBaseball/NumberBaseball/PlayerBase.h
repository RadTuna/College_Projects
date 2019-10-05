#pragma once

class PlayerBase
{
public:

	PlayerBase();
	~PlayerBase() = default;

	// true : �ߺ� �˻� ��� / false : �ߺ� �˻� ����
	// ���� 3�ڸ��� �Է��� ����. �׷��� ���� ��� false ��ȯ.
	bool CheckNumber(int InNum) const;

	virtual void InputNumber() = 0;

	void SetCurrentNumber(int InNum);

private:

	int CurrentNumber;

public:

	inline int GetCurrentNumber() const { return CurrentNumber; }

};

