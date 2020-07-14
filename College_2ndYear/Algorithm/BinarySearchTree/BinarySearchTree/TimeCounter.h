#pragma once

#include <windows.h>


class TimeCounter final
{
public:

	~TimeCounter() = default;

	static void CreateInstance();
	static TimeCounter* GetInstance();
	static void DeleteInstance();

	bool CaptureStart();
	void CaptureEnd();

private:

	explicit TimeCounter() noexcept;
	
private:

	static TimeCounter* mInstance;
	
	__int64 mFrequency;
	__int64 mStartTime;
	float mTickPerMs;
	float mFrameTime;

public:

	inline float GetDeltaTime() const { return mFrameTime; }

};