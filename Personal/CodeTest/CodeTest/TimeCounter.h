#pragma once

#include <windows.h>


class TimeCounter
{
public:

	TimeCounter();
	~TimeCounter();

	bool Initialize();
	void Frame();

private:

	__int64 mFrequency;
	__int64 mStartTime;
	double mTickPerMs;
	double mFrameTime;
	double mFramePerSecond;

public:

	inline double GetFPS() { return mFramePerSecond; }
	inline double GetDeltaTime() { return mFrameTime; }

};