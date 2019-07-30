
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

	// 스크린 높이, 너비를 저장.
	mScreenWidth = ScreenWidth;
	mScreenHeight = ScreenHeight;

	// 뷰 행렬을 저장.
	DirectX::XMStoreFloat4x4(&mBaseViewMatrix, BaseViewMatrix);

	// 폰트 객체를 생성.
	mFont = new FontClass;
	if (mFont == nullptr)
	{
		return false;
	}

	// 폰트 객체를 초기화.
	Result = mFont->Initialize(Device, DeviceContext, "../DirectXTutorials/Data/fontdata.txt", "../DirectXTutorials/Data/fontRGB.tga");
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize the font object.", "Error", MB_OK);
		return false;
	}

	// 폰트 셰이더 객체를 생성.
	mFontShader = new FontShaderClass;
	if (mFontShader == nullptr)
	{
		return false;
	}

	// 폰트 셰이더 객체를 초기화.
	Result = mFontShader->Initialize(Device, hWnd);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize the font shader object.", "Error", MB_OK);
		return false;
	}

	// 첫번째 문장을 초기화.
	Result = InitializeSentence(&mSentence1, 16, Device);
	if (Result == false)
	{
		return false;
	}

	// 문장 버텍스 버퍼를 문자열 정보로 업데이트.
	Result = UpdateSentence(mSentence1, "Hello", 100, 100, 1.0f, 1.0f, 1.0f, DeviceContext);
	if (Result == false)
	{
		return false;
	}

	// 두번째 문장을 초기화.
	Result = InitializeSentence(&mSentence2, 16, Device);
	if (Result == false)
	{
		return false;
	}

	// 문장 버텍스 버퍼를 문자열 정보로 업데이트.
	Result = UpdateSentence(mSentence2, "Goodbye", 100, 200, 1.0f, 1.0f, 0.0f, DeviceContext);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	// 첫번째 문장을 해제.
	ReleaseSentence(&mSentence1);

	// 두번째 문장을 해제.
	ReleaseSentence(&mSentence2);

	// 폰트 셰이더 객체를 해제.
	if (mFontShader != nullptr)
	{
		mFontShader->Shutdown();
		delete mFontShader;
		mFontShader = nullptr;
	}

	// 폰트 객체를 해제.
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

	// 첫번째 문장을 그림.
	Result = RenderSentence(DeviceContext, mSentence1, WorldMatrix, OthoMatrix);
	if (Result == false)
	{
		return false;
	}

	// 두번째 문장을 그림.
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

	// 문장 객체를 생성.
	*pSentence = new SentenceType;
	if (*pSentence == nullptr)
	{
		return false;
	}

	// 문장 버퍼를 null로 초기화.
	(*pSentence)->VertexBuffer = nullptr;
	(*pSentence)->IndexBuffer = nullptr;

	// 문장의 최대 길이를 설정.
	(*pSentence)->MaxLength = MaxLength;

	// 문장의 버텍스 숫자를 설정.
	(*pSentence)->VertexCount = MaxLength * 6;

	// 문장의 인덱스 숫자를 설정.
	(*pSentence)->IndexCount = (*pSentence)->VertexCount;

	// 버텍스 배열을 생성.
	Vertices = new VertexType[(*pSentence)->VertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// 인덱스 배열을 생성.
	Indices = new unsigned long[(*pSentence)->IndexCount];
	if (Indices == nullptr)
	{
		return false;
	}

	// 버텍스 배열을 0으로 초기화.
	memset(Vertices, 0, sizeof(VertexType) * (*pSentence)->VertexCount);

	// 인덱스 배열을 초기화.
	for (int i = 0; i < (*pSentence)->IndexCount; ++i)
	{
		Indices[i] = i;
	}

	// 동적 버텍스 버퍼의 Description을 작성.
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * (*pSentence)->VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// 서브리소스 구조체에 버텍스 데이터의 포인터를 저장.
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// 버텍스 버퍼를 생성.
	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &(*pSentence)->VertexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// 정적 인덱스 버퍼의 Description을 작성.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * (*pSentence)->IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// 서브리소스 구조체에 인덱스 데이터의 포인터를 저장.
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성.
	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &(*pSentence)->IndexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// 필요하지 않은 버텍스-인덱스 버퍼를 해제.
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

	// 문장의 색상을 저장.
	Sentence->Red = Red;
	Sentence->Green = Green;
	Sentence->Blue = Blue;

	// 문장내의 문자의 개수를 얻음.
	NumLetters = static_cast<int>(strlen(Text));

	// 버퍼의 오버플로우 유무를 검사.
	if (NumLetters > Sentence->MaxLength)
	{
		return false;
	}

	// 버텍스 배열을 생성.
	Vertices = new VertexType[Sentence->VertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// 버텍스 배열을 0으로 초기화.
	memset(Vertices, 0, sizeof(VertexType) * Sentence->VertexCount);

	// 그리기를 시작할 화면에서 X, Y좌표 위치를 계산.
	DrawX = static_cast<float>(((mScreenWidth / 2) * -1) + PosX);
	DrawY = static_cast<float>((mScreenHeight / 2) - PosY);

	// 폰트 클래스를 사용해 버텍스 배열을 생성.
	mFont->BuildVertexArray(reinterpret_cast<void*>(Vertices), Text, DrawX, DrawY);

	// 수정을 위해 버텍스 버퍼를 잠금.
	Result = DeviceContext->Map(Sentence->VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	// 버텍스 버퍼에 있는 데이터에 대한 포인터를 가져옴.
	VerticesPtr = reinterpret_cast<VertexType*>(MappedResource.pData);

	// 버텍스 버퍼로 데이터를 복사함.
	memcpy(VerticesPtr, reinterpret_cast<void*>(Vertices), sizeof(VertexType) * Sentence->VertexCount);

	// 버텍스 버퍼의 잠금을 해제.
	DeviceContext->Unmap(Sentence->VertexBuffer, 0);

	// 더이상 사용하지 않는 버텍스 배열을 해제.
	delete[] Vertices;
	Vertices = nullptr;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** pSentence)
{
	if (*pSentence != nullptr)
	{
		// 문장 버텍스 버퍼를 해제.
		if ((*pSentence)->VertexBuffer != nullptr)
		{
			(*pSentence)->VertexBuffer->Release();
			(*pSentence)->VertexBuffer = nullptr;
		}

		// 문장 인덱스 버퍼를 해제.
		if ((*pSentence)->IndexBuffer != nullptr)
		{
			(*pSentence)->IndexBuffer->Release();
			(*pSentence)->IndexBuffer = nullptr;
		}

		// 문장을 해제.
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

	// 버텍스 버퍼의 Stride, Offset값을 설정.
	Stride = sizeof(VertexType);
	Offset = 0;

	// Input Assembler(GPU)에 버텍스 버퍼를 활성화하여 렌더링을 가능하게 함.
	DeviceContext->IASetVertexBuffers(0, 1, &Sentence->VertexBuffer, &Stride, &Offset);

	// Input Assembler에 인덱스 버퍼를 활성화하여 렌더링을 가능하게 함.
	DeviceContext->IASetIndexBuffer(Sentence->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 버텍스 버퍼로 그릴 기본형을 설정. 여기서는 삼각형임.
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 문장색상으로 입력할 픽셀색상을 설정.
	PixelColor = DirectX::XMVectorSet(Sentence->Red, Sentence->Green, Sentence->Blue, 1.0f);

	BaseViewMatrix = DirectX::XMLoadFloat4x4(&mBaseViewMatrix);

	// 폰트셰이더를 사용해 렌더링.
	Result = mFontShader->Render(DeviceContext, Sentence->IndexCount, WorldMatrix, BaseViewMatrix, OrthoMatrix, mFont->GetTexture(), PixelColor);
	if (Result == false)
	{
		return false;
	}

	return true;
}

