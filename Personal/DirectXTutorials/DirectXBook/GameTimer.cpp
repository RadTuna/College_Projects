
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
		// ���� ������ �ð��� ������ ������ �ð�(�Ǵ� ���۽ð�)���� ��ü �ð��� ��ȯ.
		return static_cast<float>(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	else
	{
		// ���� �ð��� ������ ������ �ð�(�Ǵ� ���۽ð�)���� ��ü �ð��� ��ȯ.
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

	// BaseTime�� PrevTime�� CurrTime���� �ʱ�ȭ.
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
		// �����Ǿ����� �ð��� �����ص�.
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
		// Ÿ�̸Ӱ� ���� �����̸� DeltaTime�� 0���� ��.
		mDeltaTime = 0.0;
		return;
	}

	__int64 CurrTime;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&CurrTime));
	mCurrTime = CurrTime;

	// ���� ƽ�� ���� ƽ�� ���� ���ϰ� PerCount�� ���� �ʸ� ����.
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;
	mPrevTime = mCurrTime; // PrevTime�� ����

	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}
