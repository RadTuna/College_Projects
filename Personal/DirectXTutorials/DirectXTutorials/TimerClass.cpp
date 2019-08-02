
#include "TimerClass.h"


TimerClass::TimerClass()
{
}

TimerClass::TimerClass(const TimerClass& Other)
{
}

TimerClass::~TimerClass()
{
}

bool TimerClass::Initialize()
{
	// 해당 시스템이 고성능 타이머를 지원하는지 확인.
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&mFrequency));
	if (mFrequency == 0)
	{
		return false;
	}

	// 주파수 카운터의 밀리초 당 틱 횟수를 확인.
	mTicksPerMs = static_cast<float>(mFrequency / 1000);

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mStartTime));

	return true;
}

void TimerClass::Frame()
{
	__int64 CurrentTime;
	float TimeDifference;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&CurrentTime));

	TimeDifference = static_cast<float>(CurrentTime - mStartTime);

	mFrameTime = TimeDifference / mTicksPerMs;

	mStartTime = CurrentTime;

	return;
}