#include <iostream>
#include <memory>
#include "TestUniquePointer.h"
#include "Vector.h"

using namespace std;

namespace Test
{
	void ExecUniquePointer()
	{
		// ����ũ �����͸� ���� �� �ʱ�ȭ��.
		unique_ptr<char> charTest = make_unique<char>('A');
		unique_ptr<int> intTest = make_unique<int>(66);

		cout << "CharTest : " << *charTest << endl;
		cout << "IntTest : " << *intTest << endl;
		cout << endl;

		// ����ũ �����͸� Vector��ü�� ������.
		unique_ptr<Vector> MyVector = make_unique<Vector>(15, 51);
		cout << "X : " << MyVector->GetX() << "  /  Y : " << MyVector->GetY() << endl;

		// ����ũ �������� �ּҰ��� ���ο� ��ü�� �ּҷ� ��ü��.
		MyVector.reset(new Vector(91, 19));
		cout << "X : " << MyVector->GetX() << "  /  Y : " << MyVector->GetY() << endl;

		// ����ũ �������� �������� move�Լ��� �̿��� ��������
		// make_unique ���ø��� �̿��ϸ� ������ ������ �߻���.
		unique_ptr<Vector> MyVector2 (move(MyVector));
		cout << "X2 : " << MyVector2->GetX() << "  /  Y2 : " << MyVector2->GetY() << endl;
		
		// ����ũ �����͸� �Ҹ��Ŵ. nullptr���԰� ����.
		MyVector.reset();

		// ����ũ �����͸� const(���)�� �����ϸ�
		// reset�� move����� �Ұ�����.
	}

	// ����ũ �������� ���� �����ڴ� delete�Ǿ� �־� �Լ� �Ű������� �ѱ� �� ����.
	// ���� ���� �����ڸ� ������� �ʴ� ���۷����� �Ѱ���� �Ѵ�.
	bool UniquePointerParameter(unique_ptr<int>& Para1)
	{
		if (*Para1 > 0)
		{
			return true;
		}
		return false;
	}
}
