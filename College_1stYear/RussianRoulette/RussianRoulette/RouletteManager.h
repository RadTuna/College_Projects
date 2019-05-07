#pragma once

class Revolver;

class RouletteManager final
{
public:
	RouletteManager() = delete;

	static void SetMode(bool RandPlayer, bool RandShuffle, bool LastOne);
	static void PlayRoulette(Revolver& Pistol, int Player);

private:
	// True�̸� ù �����÷��̾ ������ �˴ϴ�.
	static bool IsRandPlayer;

	// True�̸� �ѹ� �� ���� źâ�� ȸ���մϴ�. (����)
	static bool IsRandShuffle;

	// True�̸� ź���� ������ �� ���� ������ ����մϴ�.
	static bool IsLastOneMode;
};