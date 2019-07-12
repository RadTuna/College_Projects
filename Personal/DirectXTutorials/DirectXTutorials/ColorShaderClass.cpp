
#include "ColorShaderClass.h"


ColorShaderClass::ColorShaderClass()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mLayout = nullptr;
	mMatrixBuffer = nullptr;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass&)
{
}

ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize(ID3D11Device* Device, HWND hWnd)
{
	bool Result;

	// 버텍스 셰이더와 픽셀 셰이더를 초기화.
	Result = InitializeShader(Device, hWnd, const_cast<WCHAR*>(L"../DirectXTutorials/ColorVS.vs"), const_cast<WCHAR*>(L"../DirectXTutorials/ColorPS.ps"));
	if (Result == false)
	{
		return false;
	}

	return true;
}

void ColorShaderClass::Shutdown()
{
	// 버텍스 셰이더와 픽셀 셰이더 및 그와 관련된 것들을 해제.
	ShutdownShader();
	
	return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext* DeviceContext, int IndexCount, DirectX::XMMATRIX& WorldMat, DirectX::XMMATRIX& ViewMat, DirectX::XMMATRIX& ProjectionMat)
{
	bool Result;

	// 렌더링에 사용할 셰이더의 인자를 입력.
	Result = SetShaderParameters(DeviceContext, WorldMat, ViewMat, ProjectionMat);
	if (Result == false)
	{
		return false;
	}

	// 셰이더를 이용하여 준비된 버퍼를 그림.
	RenderShader(DeviceContext, IndexCount);

	return true;
}

// 셰이더 파일을 불러와 DirectX와 GPU에서 사용가능 하도록 하는 함수 (중요한 함수임)
bool ColorShaderClass::InitializeShader(ID3D11Device* Device, HWND hWnd, WCHAR* VsFileName, WCHAR* PsFileName)
{
	HRESULT Result;
	ID3DBlob* ErrorMessage;
	ID3DBlob* VertexShaderBuffer;
	ID3DBlob* PixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
	unsigned int NumElements;
	D3D11_BUFFER_DESC MatrixBufferDesc;

	// 이 함수에서 사용하는 포인터를 초기화
	ErrorMessage = nullptr;
	VertexShaderBuffer = nullptr;
	PixelShaderBuffer = nullptr;

	// 여기서 셰이더 프로그램을 버퍼로 컴파일함.

	// 버텍스 셰이더를 컴파일.
	Result = D3DCompileFromFile(VsFileName, nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage);
	if (FAILED(Result))
	{
		// 셰이더 컴파일에 실패하면 에러 메세지를 기록함.
		if (ErrorMessage != nullptr)
		{
			OutputShaderErrorMessage(ErrorMessage, hWnd, VsFileName);
		}
		// 에러 메세지가 없다면 셰이더 파일을 찾지 못한것.
		else
		{
			MessageBoxW(hWnd, VsFileName, L"Missing VertexShader File", MB_OK);
		}

		return false;
	}


	// 픽셀 셰이더를 컴파일.
	Result = D3DCompileFromFile(PsFileName, nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &ErrorMessage);
	if (FAILED(Result))
	{
		// 셰이더 컴파일이 실패하면 에러 메세지를 기록함.
		if (ErrorMessage != nullptr)
		{
			OutputShaderErrorMessage(ErrorMessage, hWnd, PsFileName);
		}
		// 에러 메세지가 없다면 셰이더 파일을 찾지 못한것.
		else
		{
			MessageBoxW(hWnd, VsFileName, L"Missing PixelShader File", MB_OK);
		}

		return false;
	}

	// 버퍼를 이용하여 클래스변수로 있는 버텍스-픽셀 셰이더를 생성함.

	// 버퍼로부터 버텍스 셰이더를 생성.
	Result = Device->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), nullptr, &mVertexShader);
	if (FAILED(Result))
	{
		return false;
	}

	// 버퍼로부터 픽셀 셰이더를 생성.
	Result = Device->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), nullptr, &mPixelShader);
	if (FAILED(Result))
	{
		return false;
	}

	// 버텍스 입력 레이아웃 Description을 작성.
	// 이 설정은 ModelClass와 셰이더에 있는 VertexType과 일치해야 함.
	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[1].SemanticName = "COLOR";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	// 레이아웃의 요소 개수를 가져옴.
	NumElements = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);

	// 버텍스 입력 레이아웃을 생성.
	Result = Device->CreateInputLayout(PolygonLayout, NumElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &mLayout);
	if (FAILED(Result))
	{
		return false;
	}

	// 사용되지 않는 버텍스-픽셀 셰이더 버퍼를 해제.
	VertexShaderBuffer->Release();
	VertexShaderBuffer = nullptr;

	PixelShaderBuffer->Release();
	PixelShaderBuffer = nullptr;
	
	// 버텍스 셰이더에 있는 행렬 상수 버퍼의 Description을 작성.
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있도록 함.
	Result = Device->CreateBuffer(&MatrixBufferDesc, nullptr, &mMatrixBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	return true;
}

void ColorShaderClass::ShutdownShader()
{
	// 상수 버퍼를 해제.
	if (mMatrixBuffer != nullptr)
	{
		mMatrixBuffer->Release();
		mMatrixBuffer = nullptr;
	}

	// 레이아웃을 해제.
	if (mLayout)
	{
		mLayout->Release();
		mLayout = nullptr;
	}

	// 픽셀 셰이더를 해제.
	if (mPixelShader != nullptr)
	{
		mPixelShader->Release();
		mPixelShader = nullptr;
	}

	// 버텍스 셰이더를 해제.
	if (mVertexShader != nullptr)
	{
		mVertexShader->Release();
		mVertexShader = nullptr;
	}

	return;
}

// 버텍스-픽셀 셰이더의 에러 메세지를 출력함.
void ColorShaderClass::OutputShaderErrorMessage(ID3DBlob* ErrorMessage, HWND hWnd, WCHAR* ShaderFileName)
{
	char* CompileErrors;
	unsigned __int64 bufferSize;
	unsigned __int64 i;
	std::ofstream Fout;

	// 에러 메세지를 담고 있는 문자열 버퍼의 포인터를 가져옴.
	CompileErrors = reinterpret_cast<char*>(ErrorMessage->GetBufferPointer());

	// 메세지의 길이를 가져옴.
	bufferSize = ErrorMessage->GetBufferSize();

	// 파일을 열고 안에 메세지를 기록.
	Fout.open("shader-error.txt");

	// 에러 메세지를 씀.
	for (i = 0; i < bufferSize; ++i)
	{
		Fout << CompileErrors[i];
	}

	// 파일을 닫음.
	Fout.close();

	// 에러 메세지를 반환.
	ErrorMessage->Release();
	ErrorMessage = nullptr;

	// 컴파일 에러가 있음을 팝업 메세지로 알려줌.
	MessageBoxW(hWnd, L"Error compiling shader. Check shader-error.txt for message.", ShaderFileName, MB_OK);

	return;
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* DeviceContext, DirectX::XMMATRIX& WorldMat, DirectX::XMMATRIX& ViewMat, DirectX::XMMATRIX& ProjectionMat)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	MatrixBufferType* DataPtr;
	unsigned int BufferNumber;

	// 행렬을 전치하여 셰이더에서 사용할 수 있도록 함.
	WorldMat = DirectX::XMMatrixTranspose(WorldMat);
	ViewMat = DirectX::XMMatrixTranspose(ViewMat);
	ProjectionMat = DirectX::XMMatrixTranspose(ProjectionMat);

	// 상수 버퍼의 내용을 쓸 수 있도록 잠금.
	Result = DeviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	// 상수버퍼에 데이터에 대한 포인터를 가져옴.
	DataPtr = reinterpret_cast<MatrixBufferType*>(MappedResource.pData);

	// 상수 버퍼에 행렬을 복사함.
	DirectX::XMStoreFloat4x4(&DataPtr->World, WorldMat);
	DirectX::XMStoreFloat4x4(&DataPtr->View, ViewMat);
	DirectX::XMStoreFloat4x4(&DataPtr->Projection, ProjectionMat);

	// 상수 버퍼의 잠금을 품.
	DeviceContext->Unmap(mMatrixBuffer, 0);

	// 정점 셰이더에서의 상수 버퍼의 위치를 설정.
	BufferNumber = 0;

	// 마지막으로 정점 셰이더의 버퍼를 바뀐값으로 바꿈.
	DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &mMatrixBuffer);

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* DeviceContext, int IndexCount)
{
	// 정점 입력 레이아웃을 설정.
	DeviceContext->IASetInputLayout(mLayout);

	// 삼각형을 그릴 정점 셰이더와 픽셀 셰이더를 설정.
	DeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(mPixelShader, nullptr, 0);

	// 삼각형을 그림.
	DeviceContext->DrawIndexed(IndexCount, 0, 0);
}

