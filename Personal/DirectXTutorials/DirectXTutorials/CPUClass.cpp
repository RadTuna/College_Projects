
#include "CPUClass.h"


CPUClass::CPUClass()
{
}

CPUClass::CPUClass(const CPUClass& Other)
{
}

CPUClass::~CPUClass()
{
}

void CPUClass::Initialize()
{
	PDH_STATUS Status;

	// CPU를 읽을 수 있는지 없는지를 나타내는 플래그를 초기화.
	mCanReadCpu = true;

	// Cpu에 폴링할 쿼리 객체를 생성.
	Status = PdhOpenQuery(nullptr,0 , &mQueryHandle);
	if (Status != ERROR_SUCCESS)
	{
		mCanReadCpu = false;
	}

	// 모든 CPU를 폴링하도록 쿼리 객체를 설정.
	Status = PdhAddCounter(mQueryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &mCounterHandle);
	if (Status != ERROR_SUCCESS)
	{
		mCanReadCpu = false;
	}

	mLastSampleTime = GetTickCount();

	mCpuUsage = 0;

	return;
}

void CPUClass::Shutdown()
{
	if (mCanReadCpu == true)
	{
		PdhCloseQuery(mQueryHandle);
	}

	return;
}

void CPUClass::Frame()
{
	PDH_FMT_COUNTERVALUE Value;

	if (mCanReadCpu == true)
	{
		if (mLastSampleTime + 1000 < GetTickCount())
		{
			mLastSampleTime = GetTickCount();

			PdhCollectQueryData(mQueryHandle);

			PdhGetFormattedCounterValue(mCounterHandle, PDH_FMT_LONG, nullptr, &Value);

			mCpuUsage = Value.longValue;
		}
	}

	return;
}