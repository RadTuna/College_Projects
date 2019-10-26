#pragma once

#include <ctime>
#include <Windows.h>

class Timer
{
public:

	Timer();
	~Timer();

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
