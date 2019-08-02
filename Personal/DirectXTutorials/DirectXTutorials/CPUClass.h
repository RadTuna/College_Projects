#pragma once


#pragma comment(lib, "pdh.lib")


#include <Pdh.h>


class CPUClass
{
public:

	CPUClass();
	CPUClass(const CPUClass&);
	~CPUClass();

	void Initialize();
	void Shutdown();
	void Frame();

private:

	bool mCanReadCpu;
	HQUERY mQueryHandle;
	HCOUNTER mCounterHandle;
	unsigned long mLastSampleTime;
	long mCpuUsage;

public:

	inline int GetCpuPercentage() const
	{
		if (mCanReadCpu == true)
			return static_cast<int>(mCpuUsage);
		else
			return 0;
	}

};
