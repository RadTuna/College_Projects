
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

	// ��� Ű���� ������ ���� ���·� �ʱ�ȭ.
	for (i = 0; i < 256; ++i)
	{
		mKeys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int Input)
{
	// Ű�� ���ȴٸ� �� ���¸� �迭�� ����.
	mKeys[Input] = true;
	return;
}

void InputClass::KeyUp(unsigned int Input)
{
	// Ű�� �������ٸ� �� ���¸� �迭�� ����.
	mKeys[Input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int Key)
{
	// ���� Ű�� ���¸� ��ȯ.
	return mKeys[Key];
}
