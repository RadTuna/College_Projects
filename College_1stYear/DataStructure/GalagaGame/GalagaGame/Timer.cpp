#include "Timer.h"

Timer::Timer()
	: IntervalSec(0),
	CurrentSec(0)
{
}

void Timer::SetTimer(long MilliSec, void* InClassPtr ,void(*pFunc)(void*))
{
	IntervalSec = MilliSec;
	mFunc = pFunc;
	ClassPtr = InClassPtr;
	CurrentSec = clock();

	ThreadHandle = CreateThread(nullptr, 0, LoopTimer, this, 0, nullptr);
}

void Timer::ClearTimer()
{
	TerminateThread(ThreadHandle, 0);
}

DWORD __stdcall Timer::LoopTimer(LPVOID arg)
{
	Timer* pThis = reinterpret_cast<Timer*>(arg);

	while (true)
	{
		if (pThis->CurrentSec + pThis->IntervalSec <= clock())
		{
			pThis->mFunc(pThis->ClassPtr);
			pThis->CurrentSec = clock();
		}
	}
}
