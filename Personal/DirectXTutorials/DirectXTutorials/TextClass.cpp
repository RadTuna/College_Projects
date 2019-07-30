
#include "TextClass.h"


TextClass::TextClass()
{
	mFont = nullptr;
	mFontShader = nullptr;
	mSentence1 = nullptr;
	mSentence2 = nullptr;
}

TextClass::TextClass(const TextClass& Other)
{
}

TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, HWND hWnd,
	int ScreenWidth, int ScreenHeight, DirectX::FXMMATRIX BaseViewMatrix)
{
	bool Result;

	// ��ũ�� ����, �ʺ� ����.
	mScreenWidth = ScreenWidth;
	mScreenHeight = ScreenHeight;

	// �� ����� ����.
	DirectX::XMStoreFloat4x4(&mBaseViewMatrix, BaseViewMatrix);

	// ��Ʈ ��ü�� ����.
	mFont = new FontClass;
	if (mFont == nullptr)
	{
		return false;
	}

	// ��Ʈ ��ü�� �ʱ�ȭ.
	Result = mFont->Initialize(Device, DeviceContext, "../DirectXTutorials/Data/fontdata.txt", "../DirectXTutorials/Data/fontRGB.tga");
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize the font object.", "Error", MB_OK);
		return false;
	}

	// ��Ʈ ���̴� ��ü�� ����.
	mFontShader = new FontShaderClass;
	if (mFontShader == nullptr)
	{
		return false;
	}

	// ��Ʈ ���̴� ��ü�� �ʱ�ȭ.
	Result = mFontShader->Initialize(Device, hWnd);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize the font shader object.", "Error", MB_OK);
		return false;
	}

	// ù��° ������ �ʱ�ȭ.
	Result = InitializeSentence(&mSentence1, 16, Device);
	if (Result == false)
	{
		return false;
	}

	// ���� ���ؽ� ���۸� ���ڿ� ������ ������Ʈ.
	Result = UpdateSentence(mSentence1, "Hello", 100, 100, 1.0f, 1.0f, 1.0f, DeviceContext);
	if (Result == false)
	{
		return false;
	}

	// �ι�° ������ �ʱ�ȭ.
	Result = InitializeSentence(&mSentence2, 16, Device);
	if (Result == false)
	{
		return false;
	}

	// ���� ���ؽ� ���۸� ���ڿ� ������ ������Ʈ.
	Result = UpdateSentence(mSentence2, "Goodbye", 100, 200, 1.0f, 1.0f, 0.0f, DeviceContext);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	// ù��° ������ ����.
	ReleaseSentence(&mSentence1);

	// �ι�° ������ ����.
	ReleaseSentence(&mSentence2);

	// ��Ʈ ���̴� ��ü�� ����.
	if (mFontShader != nullptr)
	{
		mFontShader->Shutdown();
		delete mFontShader;
		mFontShader = nullptr;
	}

	// ��Ʈ ��ü�� ����.
	if (mFont != nullptr)
	{
		mFont->Shutdown();
		delete mFont;
		mFont = nullptr;
	}

	return;
}

bool TextClass::Render(ID3D11DeviceContext* DeviceContext, DirectX::XMMATRIX& WorldMatrix, DirectX::XMMATRIX& OthoMatrix)
{
	bool Result;

	// ù��° ������ �׸�.
	Result = RenderSentence(DeviceContext, mSentence1, WorldMatrix, OthoMatrix);
	if (Result == false)
	{
		return false;
	}

	// �ι�° ������ �׸�.
	Result = RenderSentence(DeviceContext, mSentence2, WorldMatrix, OthoMatrix);
	if (Result == false)
	{
		return false;
	}

	return true;
}

bool TextClass::InitializeSentence(SentenceType** pSentence, int MaxLength, ID3D11Device* Device)
{
	VertexType* Vertices;
	unsigned long* Indices;
	D3D11_BUFFER_DESC VertexBufferDesc;
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData;
	D3D11_SUBRESOURCE_DATA IndexData;
	HRESULT Result;

	// ���� ��ü�� ����.
	*pSentence = new SentenceType;
	if (*pSentence == nullptr)
	{
		return false;
	}

	// ���� ���۸� null�� �ʱ�ȭ.
	(*pSentence)->VertexBuffer = nullptr;
	(*pSentence)->IndexBuffer = nullptr;

	// ������ �ִ� ���̸� ����.
	(*pSentence)->MaxLength = MaxLength;

	// ������ ���ؽ� ���ڸ� ����.
	(*pSentence)->VertexCount = MaxLength * 6;

	// ������ �ε��� ���ڸ� ����.
	(*pSentence)->IndexCount = (*pSentence)->VertexCount;

	// ���ؽ� �迭�� ����.
	Vertices = new VertexType[(*pSentence)->VertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// �ε��� �迭�� ����.
	Indices = new unsigned long[(*pSentence)->IndexCount];
	if (Indices == nullptr)
	{
		return false;
	}

	// ���ؽ� �迭�� 0���� �ʱ�ȭ.
	memset(Vertices, 0, sizeof(VertexType) * (*pSentence)->VertexCount);

	// �ε��� �迭�� �ʱ�ȭ.
	for (int i = 0; i < (*pSentence)->IndexCount; ++i)
	{
		Indices[i] = i;
	}

	// ���� ���ؽ� ������ Description�� �ۼ�.
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * (*pSentence)->VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// ���긮�ҽ� ����ü�� ���ؽ� �������� �����͸� ����.
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// ���ؽ� ���۸� ����.
	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &(*pSentence)->VertexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// ���� �ε��� ������ Description�� �ۼ�.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * (*pSentence)->IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// ���긮�ҽ� ����ü�� �ε��� �������� �����͸� ����.
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ����.
	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &(*pSentence)->IndexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// �ʿ����� ���� ���ؽ�-�ε��� ���۸� ����.
	delete[] Vertices;
	Vertices = nullptr;

	delete[] Indices;
	Indices = nullptr;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* Sentence, const char* Text, int PosX, int PosY, float Red, float Green, float Blue, ID3D11DeviceContext* DeviceContext)
{
	int NumLetters;
	VertexType* Vertices;
	float DrawX;
	float DrawY;
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	VertexType* VerticesPtr;

	// ������ ������ ����.
	Sentence->Red = Red;
	Sentence->Green = Green;
	Sentence->Blue = Blue;

	// ���峻�� ������ ������ ����.
	NumLetters = static_cast<int>(strlen(Text));

	// ������ �����÷ο� ������ �˻�.
	if (NumLetters > Sentence->MaxLength)
	{
		return false;
	}

	// ���ؽ� �迭�� ����.
	Vertices = new VertexType[Sentence->VertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// ���ؽ� �迭�� 0���� �ʱ�ȭ.
	memset(Vertices, 0, sizeof(VertexType) * Sentence->VertexCount);

	// �׸��⸦ ������ ȭ�鿡�� X, Y��ǥ ��ġ�� ���.
	DrawX = static_cast<float>(((mScreenWidth / 2) * -1) + PosX);
	DrawY = static_cast<float>((mScreenHeight / 2) - PosY);

	// ��Ʈ Ŭ������ ����� ���ؽ� �迭�� ����.
	mFont->BuildVertexArray(reinterpret_cast<void*>(Vertices), Text, DrawX, DrawY);

	// ������ ���� ���ؽ� ���۸� ���.
	Result = DeviceContext->Map(Sentence->VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	// ���ؽ� ���ۿ� �ִ� �����Ϳ� ���� �����͸� ������.
	VerticesPtr = reinterpret_cast<VertexType*>(MappedResource.pData);

	// ���ؽ� ���۷� �����͸� ������.
	memcpy(VerticesPtr, reinterpret_cast<void*>(Vertices), sizeof(VertexType) * Sentence->VertexCount);

	// ���ؽ� ������ ����� ����.
	DeviceContext->Unmap(Sentence->VertexBuffer, 0);

	// ���̻� ������� �ʴ� ���ؽ� �迭�� ����.
	delete[] Vertices;
	Vertices = nullptr;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** pSentence)
{
	if (*pSentence != nullptr)
	{
		// ���� ���ؽ� ���۸� ����.
		if ((*pSentence)->VertexBuffer != nullptr)
		{
			(*pSentence)->VertexBuffer->Release();
			(*pSentence)->VertexBuffer = nullptr;
		}

		// ���� �ε��� ���۸� ����.
		if ((*pSentence)->IndexBuffer != nullptr)
		{
			(*pSentence)->IndexBuffer->Release();
			(*pSentence)->IndexBuffer = nullptr;
		}

		// ������ ����.
		delete *pSentence;
		*pSentence = nullptr;
	}

	return;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* DeviceContext, SentenceType* Sentence, DirectX::XMMATRIX& WorldMatrix, DirectX::XMMATRIX& OrthoMatrix)
{
	unsigned int Stride;
	unsigned int Offset;
	DirectX::XMVECTOR PixelColor;
	DirectX::XMMATRIX BaseViewMatrix;
	bool Result;

	// ���ؽ� ������ Stride, Offset���� ����.
	Stride = sizeof(VertexType);
	Offset = 0;

	// Input Assembler(GPU)�� ���ؽ� ���۸� Ȱ��ȭ�Ͽ� �������� �����ϰ� ��.
	DeviceContext->IASetVertexBuffers(0, 1, &Sentence->VertexBuffer, &Stride, &Offset);

	// Input Assembler�� �ε��� ���۸� Ȱ��ȭ�Ͽ� �������� �����ϰ� ��.
	DeviceContext->IASetIndexBuffer(Sentence->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���ؽ� ���۷� �׸� �⺻���� ����. ���⼭�� �ﰢ����.
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ����������� �Է��� �ȼ������� ����.
	PixelColor = DirectX::XMVectorSet(Sentence->Red, Sentence->Green, Sentence->Blue, 1.0f);

	BaseViewMatrix = DirectX::XMLoadFloat4x4(&mBaseViewMatrix);

	// ��Ʈ���̴��� ����� ������.
	Result = mFontShader->Render(DeviceContext, Sentence->IndexCount, WorldMatrix, BaseViewMatrix, OrthoMatrix, mFont->GetTexture(), PixelColor);
	if (Result == false)
	{
		return false;
	}

	return true;
}

