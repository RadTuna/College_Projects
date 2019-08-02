
#include "FontClass.h"


FontClass::FontClass()
{
	mFont = nullptr;
	mTexture = nullptr;
}

FontClass::FontClass(const FontClass& Other)
{
}

FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FontFileName, const char* TextureFileName)
{
	bool Result;

	// 폰트 데이터를 불러옴.
	Result = LoadFontData(FontFileName);
	if (Result == false)
	{
		return false;
	}

	// 글자에 대한 텍스쳐를 불러옴.
	Result = LoadTexture(Device, DeviceContext, TextureFileName);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	// 폰트 텍스쳐를 해제함.
	ReleaseTexture();

	// 폰트 데이터를 해제함.
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(const char* FileName)
{
	std::ifstream Fin;
	char TempChar;

	// 폰트 버퍼를 생성.
	mFont = new FontType[95];
	if (mFont == nullptr)
	{
		return false;
	}

	// 폰트의 사이즈, 자간을 읽음.
	Fin.open(FileName);
	if (Fin.fail())
	{
		return false;
	}

	// 95개의 아스키문자를 읽음.
	for (int i = 0; i < 95; ++i)
	{
		Fin.get(TempChar);
		while (TempChar != ' ')
		{
			Fin.get(TempChar);
		}
		Fin.get(TempChar);
		while (TempChar != ' ')
		{
			Fin.get(TempChar);
		}

		Fin >> mFont[i].Left;
		Fin >> mFont[i].Right;
		Fin >> mFont[i].Size;
	}

	// 파일을 닫음.
	Fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	// 폰트 데이터 배열을 해제.
	if (mFont != nullptr)
	{
		delete[] mFont;
		mFont = nullptr;
	}

	return;
}

bool FontClass::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FileName)
{
	bool Result;

	// 텍스쳐 객체를 생성.
	mTexture = new TextureClass;
	if (mTexture == false)
	{
		return false;
	}

	// 텍스쳐 객체를 초기화.
	Result = mTexture->Initialize(Device, DeviceContext, FileName);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	// 텍스쳐 객체를 해제함.
	if (mTexture != nullptr)
	{
		mTexture->Shutdown();
		delete mTexture;
		mTexture = nullptr;
	}

	return;
}

void FontClass::BuildVertexArray(void* Vertices, const char* Sentence, float DrawX, float DrawY)
{
	VertexType* VertexPtr;
	int NumLetters;
	int Index;
	int Letter;

	// 입력된 정점변수를 VertexType구조체로 강제 캐스팅.
	VertexPtr = reinterpret_cast<VertexType*>(Vertices);
	
	// 문장의 길이를 얻음.
	NumLetters = static_cast<int>(strlen(Sentence));

	// 버텍스 배열의 인덱스를 초기화.
	Index = 0;

	// 각 글자를 쿼드 위에 그림.
	for (int i = 0; i < NumLetters; ++i)
	{
		Letter = static_cast<int>(Sentence[i]) - 32;

		// 글자가 공백이면 3픽셀 움직임.
		if (Letter == 0)
		{
			DrawX = DrawX + 3.0f;
		}
		else
		{
			// 쿼드내의 첫번째 트라이앵글.
			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX, DrawY, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Left, 0.0f, 0.0f, 0.0f));
			Index++;

			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX + mFont[Letter].Size, DrawY - 16, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Right, 1.0f, 0.0f, 0.0f));
			Index++;

			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX, DrawY - 16, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Left, 1.0f, 0.0f, 0.0f));
			Index++;

			// 쿼드내의 두번째 트라이앵글
			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX, DrawY, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Left, 0.0f, 0.0f, 0.0f));
			Index++;

			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX + mFont[Letter].Size, DrawY, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Right, 0.0f, 0.0f, 0.0f));
			Index++;

			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX + mFont[Letter].Size, DrawY - 16, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Right, 1.0f, 0.0f, 0.0f));
			Index++;

			// 다음 문자를 그리기 위해 x좌표를 갱신.
			DrawX = DrawX + mFont[Letter].Size + 1.0f;
		}
	}

}

