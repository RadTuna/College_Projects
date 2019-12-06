#pragma once

#include <ctime>
#include <Windows.h>

class Timer
{
public:

	explicit Timer();
	~Timer() = default;

	void SetTimer(long MilliSec, void* InClassPtr, void(*pFunc)(void*));
	void ClearTimer();

private:

	static DWORD __stdcall LoopTimer(LPVOID arg);

private:

	HANDLE ThreadHandle;
	long CurrentSec;
	long IntervalSec;
	void(*mFunc)(void*);
	void* ClassPtr;

};
