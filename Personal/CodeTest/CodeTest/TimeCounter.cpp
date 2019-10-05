
#include "TimeCounter.h"


TimeCounter::TimeCounter()
{
	mFrequency = 0;
	mStartTime = 0;
	mTickPerMs = 0.0f;
	mFrameTime = 0.0f;
	mFramePerSecond = 0;
}
TimeCounter::~TimeCounter()
{
}

bool TimeCounter::Initialize()
{
	// 현재 시스템이 QueryPerformanceCounter를 지원하는 지 확인
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&mFrequency));
	if (mFrequency == 0)
	{
		return false;
	}

	mTickPerMs = static_cast<double>(mFrequency / 1000);

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mStartTime));

	return true;
}

void TimeCounter::Frame()
{
	__int64 currentTime;
	double timeDifference;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));

	timeDifference = static_cast<double>(currentTime - mStartTime);
	mFrameTime = timeDifference / mTickPerMs;
	mFramePerSecond = mFrequency / timeDifference;

	mStartTime = currentTime;
}