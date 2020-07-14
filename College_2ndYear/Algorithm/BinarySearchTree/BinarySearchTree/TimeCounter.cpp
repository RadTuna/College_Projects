
#include "TimeCounter.h"
#include <cassert>


TimeCounter* TimeCounter::mInstance = nullptr;

TimeCounter::TimeCounter() noexcept
	: mFrequency(0)
	, mStartTime(0)
	, mTickPerMs(0.0f)
	, mFrameTime(0.0f)
{
}

void TimeCounter::CreateInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new TimeCounter();
	}
}

TimeCounter* TimeCounter::GetInstance()
{
	assert(mInstance != nullptr);
	return mInstance;
}

void TimeCounter::DeleteInstance()
{
	if (mInstance != nullptr)
	{
		delete mInstance;
		mInstance = nullptr;
	}
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