
#include <Windows.h>
#include "GameTimer.h"

GameTimer::GameTimer()
	: mSecondsPerCount(0.0),
	mDeltaTime(-1.0),
	mBaseTime(0),
	mPausedTime(0),
	mPrevTime(0),
	mCurrTime(0),
	mStopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
	mSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
}

float GameTimer::GameTime() const
{
	if (mStopped == true)
	{
		// 현재 정지된 시간과 이전에 정지된 시간(또는 시작시간)과의 전체 시간을 반환.
		return static_cast<float>(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	else
	{
		// 현재 시간과 이전에 정지된 시간(또는 시작시간)과의 전체 시간을 반환.
		return static_cast<float>(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
}

float GameTimer::DeltaTime() const
{
	return static_cast<float>(mDeltaTime);
}

void GameTimer::Reset()
{
	__int64 CurrTime;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&CurrTime));

	// BaseTime과 PrevTime을 CurrTime으로 초기화.
	mBaseTime = CurrTime;
	mPrevTime = CurrTime;
	mStopTime = 0;
	mStopped = false;
}

void GameTimer::Start()
{
	__int64 StartTime;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&StartTime));

	if (mStopped == true)
	{
		// 정지되었었던 시간을 저장해둠.
		mPausedTime += (StartTime - mStopTime);

		mPrevTime = StartTime;
		mStopTime = 0;
		mStopTime = false;
	}
}

void GameTimer::Stop()
{
	if (mStopped == false)
	{
		__int64 CurrTime;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&CurrTime));

		mStopTime = CurrTime;
		mStopped = true;
	}
}

void GameTimer::Tick()
{
	if (mStopped == true)
	{
		// 타이머가 멈춘 상태이면 DeltaTime을 0으로 셋.
		mDeltaTime = 0.0;
		return;
	}

	__int64 CurrTime;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&CurrTime));
	mCurrTime = CurrTime;

	// 현재 틱과 이전 틱의 차를 구하고 PerCount를 곱해 초를 구함.
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;
	mPrevTime = mCurrTime; // PrevTime을 갱신

	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}
