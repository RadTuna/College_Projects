#pragma once

#include <thread>
#include <chrono>


class Timer
{
public:
	explicit Timer();
	~Timer() = default;

	Timer(const Timer& other) = delete;
	Timer(Timer&& other) = delete;

	template<typename Class, typename Fn, typename... Args>
	void SetTimer(int timerInterval, Class inClass, Fn inFunc, Args... inFuncArgs);

	void ClearTimer();

private:
	template<typename Class, typename Fn, typename... Args>
	static void LoopTimer(Timer* self, Class inClass, Fn inFunc, Args... inFuncArgs);

private:
	int mTimerInterval;
	bool mbIsTimerEnd;

};

Timer::Timer()
	: mTimerInterval(0),
	mbIsTimerEnd(false)
{
}

void Timer::ClearTimer()
{
	mbIsTimerEnd = true;
}

template<typename Class, typename Fn, typename... Args>
void Timer::SetTimer(int timerInterval, Class inClass, Fn inFunc, Args... inFuncArgs)
{
	mbIsTimerEnd = false;
	mTimerInterval = timerInterval;

	std::thread TimerThread(&Timer::LoopTimer<Class, Fn, Args...>, this, inClass, inFunc, inFuncArgs...);
	TimerThread.detach();
}

template<typename Class, typename Fn, typename... Args>
void Timer::LoopTimer(Timer* self, Class inClass, Fn inFunc, Args... inFuncArgs)
{
	while (self->mbIsTimerEnd == false)
	{
		(inClass->*inFunc)(inFuncArgs...);
		std::this_thread::sleep_for(std::chrono::milliseconds(self->mTimerInterval));
	}
}
