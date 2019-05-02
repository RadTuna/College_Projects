#include <iostream>
#include <memory>
#include "TestUniquePointer.h"
#include "Vector.h"

using namespace std;

namespace Test
{
	void ExecUniquePointer()
	{
		// 유니크 포인터를 선언 및 초기화함.
		unique_ptr<char> charTest = make_unique<char>('A');
		unique_ptr<int> intTest = make_unique<int>(66);

		cout << "CharTest : " << *charTest << endl;
		cout << "IntTest : " << *intTest << endl;
		cout << endl;

		// 유니크 포인터를 Vector객체로 선언함.
		unique_ptr<Vector> MyVector = make_unique<Vector>(15, 51);
		cout << "X : " << MyVector->GetX() << "  /  Y : " << MyVector->GetY() << endl;

		// 유니크 포인터의 주소값을 새로운 객체의 주소로 교체함.
		MyVector.reset(new Vector(91, 19));
		cout << "X : " << MyVector->GetX() << "  /  Y : " << MyVector->GetY() << endl;

		// 유니크 포인터의 소유권을 move함수를 이용해 이전가능
		// make_unique 템플릿을 이용하면 컴파일 에러가 발생함.
		unique_ptr<Vector> MyVector2 (move(MyVector));
		cout << "X2 : " << MyVector2->GetX() << "  /  Y2 : " << MyVector2->GetY() << endl;
		
		// 유니크 포인터를 소멸시킴. nullptr대입과 동일.
		MyVector.reset();

		// 유니크 포인터를 const(상수)로 선언하면
		// reset과 move사용이 불가능함.
	}

	// 유니크 포인터의 복사 생성자는 delete되어 있어 함수 매개변수로 넘길 수 없다.
	// 따라서 복사 생성자를 사용하지 않는 레퍼런스로 넘겨줘야 한다.
	bool UniquePointerParameter(unique_ptr<int>& Para1)
	{
		if (*Para1 > 0)
		{
			return true;
		}
		return false;
	}
}
