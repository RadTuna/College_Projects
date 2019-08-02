
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

	// ��Ʈ �����͸� �ҷ���.
	Result = LoadFontData(FontFileName);
	if (Result == false)
	{
		return false;
	}

	// ���ڿ� ���� �ؽ��ĸ� �ҷ���.
	Result = LoadTexture(Device, DeviceContext, TextureFileName);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	// ��Ʈ �ؽ��ĸ� ������.
	ReleaseTexture();

	// ��Ʈ �����͸� ������.
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(const char* FileName)
{
	std::ifstream Fin;
	char TempChar;

	// ��Ʈ ���۸� ����.
	mFont = new FontType[95];
	if (mFont == nullptr)
	{
		return false;
	}

	// ��Ʈ�� ������, �ڰ��� ����.
	Fin.open(FileName);
	if (Fin.fail())
	{
		return false;
	}

	// 95���� �ƽ�Ű���ڸ� ����.
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

	// ������ ����.
	Fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	// ��Ʈ ������ �迭�� ����.
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

	// �ؽ��� ��ü�� ����.
	mTexture = new TextureClass;
	if (mTexture == false)
	{
		return false;
	}

	// �ؽ��� ��ü�� �ʱ�ȭ.
	Result = mTexture->Initialize(Device, DeviceContext, FileName);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	// �ؽ��� ��ü�� ������.
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

	// �Էµ� ���������� VertexType����ü�� ���� ĳ����.
	VertexPtr = reinterpret_cast<VertexType*>(Vertices);
	
	// ������ ���̸� ����.
	NumLetters = static_cast<int>(strlen(Sentence));

	// ���ؽ� �迭�� �ε����� �ʱ�ȭ.
	Index = 0;

	// �� ���ڸ� ���� ���� �׸�.
	for (int i = 0; i < NumLetters; ++i)
	{
		Letter = static_cast<int>(Sentence[i]) - 32;

		// ���ڰ� �����̸� 3�ȼ� ������.
		if (Letter == 0)
		{
			DrawX = DrawX + 3.0f;
		}
		else
		{
			// ���峻�� ù��° Ʈ���̾ޱ�.
			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX, DrawY, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Left, 0.0f, 0.0f, 0.0f));
			Index++;

			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX + mFont[Letter].Size, DrawY - 16, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Right, 1.0f, 0.0f, 0.0f));
			Index++;

			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX, DrawY - 16, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Left, 1.0f, 0.0f, 0.0f));
			Index++;

			// ���峻�� �ι�° Ʈ���̾ޱ�
			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX, DrawY, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Left, 0.0f, 0.0f, 0.0f));
			Index++;

			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX + mFont[Letter].Size, DrawY, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Right, 0.0f, 0.0f, 0.0f));
			Index++;

			DirectX::XMStoreFloat3(&VertexPtr[Index].Position, DirectX::XMVectorSet(DrawX + mFont[Letter].Size, DrawY - 16, 0.0f, 0.0f));
			DirectX::XMStoreFloat2(&VertexPtr[Index].Texture, DirectX::XMVectorSet(mFont[Letter].Right, 1.0f, 0.0f, 0.0f));
			Index++;

			// ���� ���ڸ� �׸��� ���� x��ǥ�� ����.
			DrawX = DrawX + mFont[Letter].Size + 1.0f;
		}
	}

}

