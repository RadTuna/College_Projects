#pragma once

#include <windows.h>


class TimeCounter
{
public:

	TimeCounter();
	~TimeCounter();

	bool CaptureStart();
	void CaptureEnd();

private:

	__int64 mFrequency;
	__int64 mStartTime;
	float mTickPerMs;
	float mFrameTime;

public:

	inline float GetDeltaTime() { return mFrameTime; }

};