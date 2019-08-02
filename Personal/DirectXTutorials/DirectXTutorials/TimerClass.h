#pragma once


#include <windows.h>


class TimerClass
{
public:

	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();

private:

	__int64 mFrequency;
	float mTicksPerMs;
	__int64 mStartTime;
	float mFrameTime;

public:

	inline float GetTime() const { return mFrameTime; }

};

