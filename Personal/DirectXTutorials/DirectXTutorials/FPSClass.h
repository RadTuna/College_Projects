#pragma once


#pragma comment(lib, "winmm.lib")


#include <windows.h>
#include <mmsystem.h>


class FPSClass
{
public:

	FPSClass();
	FPSClass(const FPSClass&);
	~FPSClass();

	void Initialize();
	void Frame();

private:

	int mFPS;
	int mCount;
	unsigned long mStartTime;

public:

	inline int GetFPS() const { return mFPS; }

};

