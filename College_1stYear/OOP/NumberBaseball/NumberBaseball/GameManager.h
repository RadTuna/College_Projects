#pragma once

class GameManager
{
public:

	GameManager();
	~GameManager();

	void GameStart();
	void GameUpdate();

private:

	// 숫자를 비교하여 판단하는 함수. Out이면 false를 반환.
	bool CompareNumber(int& OutStrike, int& OutBall) const;

	void PrintResult();

private:

	class PlayerBase* Pitcher;
	class PlayerBase* Hitter;
	bool IsGameEnd;

public:

	inline bool GetIsGameEnd() const { return IsGameEnd; }

};
