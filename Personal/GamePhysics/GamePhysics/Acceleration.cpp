
#include "Acceleration.h"

using namespace std;

float Acceleration::GetSecond(float InSec)
{
	return InSec * FPS;
}

float Acceleration::GetMinute(float InMin)
{
	return GetSecond(InMin * 60);
}

float Acceleration::GetHour(float InHour)
{
	return GetMinute(InHour * 60);
}

float Acceleration::GetMM(float InCM)
{
	return InCM / 10.0f;
}

float Acceleration::GetCM(float InCM)
{
	return InCM;
}

float Acceleration::GetM(float InCM)
{
	return InCM * 100;
}

float Acceleration::GetKM(float InCM)
{
	return GetM(InCM) * 1000;
}

float Acceleration::GetDistance(float InitSpeed, float Acc, float DeltaTime)
{
	return (InitSpeed * DeltaTime) + (Acc * (DeltaTime * DeltaTime)) / 2;
}

