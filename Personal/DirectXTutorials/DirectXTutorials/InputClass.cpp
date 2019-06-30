
#include "InputClass.h"


InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& Other)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	int i;

	// 모든 키들을 눌리지 않은 상태로 초기화.
	for (i = 0; i < 256; ++i)
	{
		mKeys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int Input)
{
	// 키가 눌렸다면 그 상태를 배열에 저장.
	mKeys[Input] = true;
	return;
}

void InputClass::KeyUp(unsigned int Input)
{
	// 키가 떼어졌다면 그 상태를 배열에 저장.
	mKeys[Input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int Key)
{
	// 현재 키의 상태를 반환.
	return mKeys[Key];
}
