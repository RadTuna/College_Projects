
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
	// �ش� �ý����� ���� Ÿ�̸Ӹ� �����ϴ��� Ȯ��.
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&mFrequency));
	if (mFrequency == 0)
	{
		return false;
	}

	// ���ļ� ī������ �и��� �� ƽ Ƚ���� Ȯ��.
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