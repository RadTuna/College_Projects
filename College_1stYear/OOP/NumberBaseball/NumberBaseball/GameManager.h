#pragma once

class GameManager
{
public:

	GameManager();
	~GameManager();

	void GameStart();
	void GameUpdate();

private:

	// ���ڸ� ���Ͽ� �Ǵ��ϴ� �Լ�. Out�̸� false�� ��ȯ.
	bool CompareNumber(int& OutStrike, int& OutBall) const;

	void PrintResult();

private:

	class PlayerBase* Pitcher;
	class PlayerBase* Hitter;
	bool IsGameEnd;

public:

	inline bool GetIsGameEnd() const { return IsGameEnd; }

};
