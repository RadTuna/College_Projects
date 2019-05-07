#pragma once

class Revolver;

class RouletteManager final
{
public:
	RouletteManager() = delete;

	static void SetMode(bool RandPlayer, bool RandShuffle, bool LastOne);
	static void PlayRoulette(Revolver& Pistol, int Player);

private:
	// True이면 첫 시작플레이어가 랜덤이 됩니다.
	static bool IsRandPlayer;

	// True이면 한발 쏠때 마다 탄창을 회전합니다. (셔플)
	static bool IsRandShuffle;

	// True이면 탄알이 없어질 때 까지 게임을 계속합니다.
	static bool IsLastOneMode;
};