
#include "LightShaderClass.h"


LightShaderClass::LightShaderClass()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mLayout = nullptr;
	mMatrixBuffer = nullptr;
	mSampleState = nullptr;
	mLightBuffer = nullptr;
	mCameraBuffer = nullptr;
}

LightShaderClass::LightShaderClass(const LightShaderClass&)
{								
}

LightShaderClass::~LightShaderClass()
{
}

bool LightShaderClass::Initialize(ID3D11Device* Device, HWND hWnd)
{
	bool Result;

	// 버텍스 셰이더와 픽셀 셰이더를 초기화.
	Result = InitializeShader(Device, hWnd, const_cast<WCHAR*>(L"../DirectXTutorials/Light.vs"), const_cast<WCHAR*>(L"../DirectXTutorials/Light.ps"));
	if (Result == false)
	{
		return false;
	}

	return true;
}

void LightShaderClass::Shutdown()
{
	// 버텍스 셰이더와 픽셀 셰이더 및 그와 관련된 것들을 해제.
	ShutdownShader();
	
	return;
}

bool LightShaderClass::Render(ID3D11DeviceContext* DeviceContext, int IndexCount, 
	DirectX::XMMATRIX& WorldMat, DirectX::XMMATRIX& ViewMat, DirectX::XMMATRIX& ProjectionMat, ID3D11ShaderResourceView* Texture, 
	DirectX::FXMVECTOR LightDirection, DirectX::FXMVECTOR DiffuseColor, DirectX::FXMVECTOR AmbientColor,
	DirectX::GXMVECTOR CameraPostion, DirectX::HXMVECTOR SpecularColor, float SpecularPower)
{
	bool Result;

	// 렌더링에 사용할 셰이더의 인자를 입력.
	Result = SetShaderParameters(DeviceContext, WorldMat, ViewMat, ProjectionMat, Texture, LightDirection, DiffuseColor, AmbientColor, CameraPostion, SpecularColor, SpecularPower);
	if (Result == false)
	{
		return false;
	}

	// 셰이더를 이용하여 준비된 버퍼를 그림.
	RenderShader(DeviceContext, IndexCount);

	return true;
}

// 셰이더 파일을 불러와 DirectX와 GPU에서 사용가능 하도록 하는 함수 (중요한 함수임)
bool LightShaderClass::InitializeShader(ID3D11Device* Device, HWND hWnd, WCHAR* VsFileName, WCHAR* PsFileName)
{
	HRESULT Result;
	ID3DBlob* ErrorMessage;
	ID3DBlob* VertexShaderBuffer;
	ID3DBlob* PixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[3];
	unsigned int NumElements;
	D3D11_SAMPLER_DESC SamplerDesc;
	D3D11_BUFFER_DESC MatrixBufferDesc;
	D3D11_BUFFER_DESC LightBufferDesc;
	D3D11_BUFFER_DESC CameraBufferDesc;

	// 이 함수에서 사용하는 포인터를 초기화
	ErrorMessage = nullptr;
	VertexShaderBuffer = nullptr;
	PixelShaderBuffer = nullptr;

	// 여기서 셰이더 프로그램을 버퍼로 컴파일함.

	// 버텍스 셰이더를 컴파일.
	Result = D3DCompileFromFile(VsFileName, nullptr, nullptr, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage);
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
	Result = D3DCompileFromFile(PsFileName, nullptr, nullptr, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &ErrorMessage);
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

	PolygonLayout[1].SemanticName = "TEXCOORD";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	PolygonLayout[2].SemanticName = "NORMAL";
	PolygonLayout[2].SemanticIndex = 0;
	PolygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[2].InputSlot = 0;
	PolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[2].InstanceDataStepRate = 0;

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

	// 텍스쳐 샘플러 상태의 Description을 생성.
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 1; // 이방성 필터링
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// 텍스쳐 샘플러 상태를 생성.
	Result = Device->CreateSamplerState(&SamplerDesc, &mSampleState);
	if (FAILED(Result))
	{
		return false;
	}

	// 픽셀 셰이더의 동적상수 버퍼의 Description을 작성.
	// ByteWidth는 언제나 16의 배수여야 하므로 D3D11_BIND_CONSTANT_BUFFER를 사용하지 않으면 CreateBuffer함수가 실패. 
	LightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	LightBufferDesc.ByteWidth = sizeof(LightBufferType);
	LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	LightBufferDesc.MiscFlags = 0;
	LightBufferDesc.StructureByteStride = 0;

	// 이 클래스에서 라이트 상수 버퍼에 접근 할 수 있도록 라이트 상수 버퍼의 포인터를 얻음.
	Result = Device->CreateBuffer(&LightBufferDesc, nullptr, &mLightBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// 카메라 상수 버퍼의 Description을 작성.
	CameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	CameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	CameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CameraBufferDesc.MiscFlags = 0;
	CameraBufferDesc.StructureByteStride = 0;

	// 이 클래스에서 카메라 상수 버퍼에 접근 할 수 있도록 카메라 상수 버퍼의 포인터를 얻음.
	Result = Device->CreateBuffer(&CameraBufferDesc, nullptr, &mCameraBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	return true;

}

void LightShaderClass::ShutdownShader()
{
	// 카메라 상수 버퍼를 해제.
	if (mCameraBuffer != nullptr)
	{
		mCameraBuffer->Release();
		mCameraBuffer = nullptr;
	}

	// 라이트 상수 버퍼를 해제.
	if (mLightBuffer != nullptr)
	{
		mLightBuffer->Release();
		mLightBuffer = nullptr;
	}

	// 텍스쳐 샘플러 상태를 해제.
	if (mSampleState != nullptr)
	{
		mSampleState->Release();
		mSampleState = nullptr;
	}

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
void LightShaderClass::OutputShaderErrorMessage(ID3DBlob* ErrorMessage, HWND hWnd, WCHAR* ShaderFileName)
{
	char* CompileErrors;
	unsigned __int64 bufferSize;
	std::ofstream Fout;

	// 에러 메세지를 담고 있는 문자열 버퍼의 포인터를 가져옴.
	CompileErrors = reinterpret_cast<char*>(ErrorMessage->GetBufferPointer());

	// 메세지의 길이를 가져옴.
	bufferSize = ErrorMessage->GetBufferSize();

	// 파일을 열고 안에 메세지를 기록.
	Fout.open("shader-error.txt");

	// 에러 메세지를 씀.
	for (int i = 0; i < bufferSize; ++i)
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

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* DeviceContext, 
	DirectX::XMMATRIX& WorldMat, DirectX::XMMATRIX& ViewMat, DirectX::XMMATRIX& ProjectionMat, ID3D11ShaderResourceView* Texture,
	DirectX::FXMVECTOR LightDirection, DirectX::FXMVECTOR DiffuseColor, DirectX::FXMVECTOR AmbientColor,
	DirectX::GXMVECTOR CameraPosition, DirectX::HXMVECTOR SpecularColor, float SpecularPower)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	MatrixBufferType* MatrixDataPtr;
	unsigned __int32 BufferNumber;
	LightBufferType* LightDataPtr;
	CameraBufferType* CameraDataPtr;

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
	MatrixDataPtr = reinterpret_cast<MatrixBufferType*>(MappedResource.pData);

	// 상수 버퍼에 행렬을 복사함.
	DirectX::XMStoreFloat4x4(&MatrixDataPtr->World, WorldMat);
	DirectX::XMStoreFloat4x4(&MatrixDataPtr->View, ViewMat);
	DirectX::XMStoreFloat4x4(&MatrixDataPtr->Projection, ProjectionMat);

	// 상수 버퍼의 잠금을 품.
	DeviceContext->Unmap(mMatrixBuffer, 0);

	// 정점 셰이더에서의 상수 버퍼의 위치를 설정.
	BufferNumber = 0;

	// 마지막으로 정점 셰이더의 버퍼를 바뀐값으로 바꿈.
	DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &mMatrixBuffer);

	// 픽셀 셰이더에 셰이더 텍스쳐 리소스를 할당.
	DeviceContext->PSSetShaderResources(0, 1, &Texture);

	// 라이트 상수 버퍼을 수정할 수 있도록 잠금.
	Result = DeviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	// 라이트 상수 버퍼에 대한 포인터를 가져옴.
	LightDataPtr = reinterpret_cast<LightBufferType*>(MappedResource.pData);

	// 라이트 상수 버퍼에 라이트 관련 변수의 내용을 넣음.
	DirectX::XMStoreFloat4(&LightDataPtr->AmbientColor, AmbientColor);
	DirectX::XMStoreFloat4(&LightDataPtr->DiffuseColor, DiffuseColor);
	DirectX::XMStoreFloat3(&LightDataPtr->LightDirection, LightDirection);
	DirectX::XMStoreFloat4(&LightDataPtr->SpecularColor, SpecularColor);
	LightDataPtr->SpecularPower = SpecularPower;

	// 라이트 상수 버퍼의 잠금을 품.
	DeviceContext->Unmap(mLightBuffer, 0);

	// 픽셀 셰이더에서 라이트 상수 버퍼에 대한 위치를 설정.
	BufferNumber = 0;

	// 픽셀 셰이더의 라이트 상수 버퍼의 값을 설정.
	DeviceContext->PSSetConstantBuffers(BufferNumber, 1, &mLightBuffer);

	// 카메라 상수 버퍼을 수정할 수 있도록 잠금.
	Result = DeviceContext->Map(mCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}
	
	// 카메라 상수 버퍼에 대한 포인터를 가져옴.
	CameraDataPtr = reinterpret_cast<CameraBufferType*>(MappedResource.pData);

	// 카메라 상수 버퍼에 라이트 관련 변수의 내용을 넣음.
	DirectX::XMStoreFloat3(&CameraDataPtr->CameraPosition, CameraPosition);
	CameraDataPtr->Padding = 0.0f;

	// 카메라 상수 버퍼의 잠금을 품.
	DeviceContext->Unmap(mCameraBuffer, 0);

	// 픽셀 셰이더에서 라이트 상수 버퍼에 대한 위치를 설정.
	BufferNumber = 1;

	// 픽셀 셰이더의 라이트 상수 버퍼의 값을 설정.
	DeviceContext->PSSetConstantBuffers(BufferNumber, 1, &mCameraBuffer);

	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext* DeviceContext, int IndexCount)
{
	// 정점 입력 레이아웃을 설정.
	DeviceContext->IASetInputLayout(mLayout);

	// 삼각형을 그릴 정점 셰이더와 픽셀 셰이더를 설정.
	DeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(mPixelShader, nullptr, 0);

	// 텍스쳐 샘플러 상태를 픽셀 셰이더에 할당.
	DeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// 삼각형을 그림.
	DeviceContext->DrawIndexed(IndexCount, 0, 0);
}

