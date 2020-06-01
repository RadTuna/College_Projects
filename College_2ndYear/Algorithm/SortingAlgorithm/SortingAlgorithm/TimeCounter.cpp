
#include "TimeCounter.h"


TimeCounter::TimeCounter()
{
	mFrequency = 0;
	mStartTime = 0;
	mTickPerMs = 0.0f;
	mFrameTime = 0.0f;
}
TimeCounter::~TimeCounter()
{
}

bool TimeCounter::CaptureStart()
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&mFrequency));
	if (mFrequency == 0)
	{
		return false;
	}

	mTickPerMs = static_cast<float>(mFrequency / 1000);

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mStartTime));

	return true;
}

void TimeCounter::CaptureEnd()
{
	__int64 currentTime;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));

	const float timeDifference = static_cast<float>(currentTime - mStartTime);
	mFrameTime = timeDifference / mTickPerMs;
}