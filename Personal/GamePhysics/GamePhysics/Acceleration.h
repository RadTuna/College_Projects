#pragma once

#include <iostream>

using namespace std;

class Acceleration
{
public:
	const static int FPS = 60;

	static float GetSecond(float InSec);
	static float GetMinute(float InMin);
	static float GetHour(float InHour);
	static float GetMM(float InCM);
	static float GetCM(float InCM);
	static float GetM(float InCM);
	static float GetKM(float InCM);

	static float GetDistance(float InitSpeed, float Acc, float DeltaTime);
};
