
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

	// CPU�� ���� �� �ִ��� �������� ��Ÿ���� �÷��׸� �ʱ�ȭ.
	mCanReadCpu = true;

	// Cpu�� ������ ���� ��ü�� ����.
	Status = PdhOpenQuery(nullptr,0 , &mQueryHandle);
	if (Status != ERROR_SUCCESS)
	{
		mCanReadCpu = false;
	}

	// ��� CPU�� �����ϵ��� ���� ��ü�� ����.
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