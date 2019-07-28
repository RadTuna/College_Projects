
#include "D3DClass.h"


D3DClass::D3DClass()
{
	mSwapChain = nullptr;
	mDevice = nullptr;
	mDeviceContext = nullptr;
	mRenderTargetView = nullptr;
	mDepthStencilBuffer = nullptr;
	mDepthStencilState = nullptr;
	mDepthStencilView = nullptr;
	mRasterState = nullptr;
	mDepthDisabledStencilState = nullptr;
}

D3DClass::D3DClass(const D3DClass& Other)
{
}

D3DClass::~D3DClass()
{
}

// D3D11을 초기화하는 함수.
bool D3DClass::Initialize(int ScreenWidth, int ScreenHeight, bool VSync, HWND hWnd, bool FullScreen, float ScreenDepth, float ScreenNear)
{
	HRESULT Result;
	IDXGIFactory* Factory;
	IDXGIAdapter* Adapter;
	IDXGIOutput* AdapterOutput;
	unsigned int NumModes, i, Numerator, Denominator;
	size_t StringLength;
	DXGI_MODE_DESC* DisplayModeList;
	DXGI_ADAPTER_DESC AdapterDesc;
	int Error;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	D3D_FEATURE_LEVEL FeatureLevel;
	ID3D11Texture2D* BackBufferPtr;
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	D3D11_RASTERIZER_DESC RasterDesc;
	D3D11_VIEWPORT ViewPort;
	D3D11_DEPTH_STENCIL_DESC DepthDisableStencilDesc;
	float FieldOfView;
	float ScreenAspect;

	//VSync 설정을 저장.
	mVSyncEnabled = VSync;

	// DirectX 그래픽 인터페이스 팩토리를 만듬.
	Result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));
	if (FAILED(Result))
	{
		return false;
	}

	// 팩토리 객체를 사용하여 첫번째 그래픽카드 인터페이스에 대한 어댑터를 만듬.
	Result = Factory->EnumAdapters(0, &Adapter);
	if (FAILED(Result))
	{
		return false;
	}

	// 출력에 대한 첫번째 어댑터를 나열함.
	Result = Adapter->EnumOutputs(0, &AdapterOutput);
	if (FAILED(Result))
	{
		return false;
	}

	// DXGI_FORMAT_R8G8B8A8_UNORM 모니터 출력 디스플래이 포맷에 맞는 모드의 개수를 구함.
	Result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &NumModes, nullptr);
	if (FAILED(Result))
	{
		return false;
	}

	// 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트를 생성.
	DisplayModeList = new DXGI_MODE_DESC[NumModes];
	if (DisplayModeList == nullptr)
	{
		return false;
	}

	// 디스플레이 모드에 대한 리스트 구조를 채워넣음.
	Result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &NumModes, DisplayModeList);
	if (FAILED(Result))
	{
		return false;
	}

	// 이제 모든 디스플레이 모드에 대해 화면 너비/높이에 맞는 디스플레이 모드를 찾음.
	// 적합한 모드를 찾으면 모니터의 새로고침 비율의 분모와 분자값을 저장함.
	for (i = 0; i < NumModes; ++i)
	{
		if (DisplayModeList[i].Width == static_cast<unsigned int>(ScreenWidth))
		{
			if (DisplayModeList[i].Height == static_cast<unsigned int>(ScreenHeight))
			{
				Numerator = DisplayModeList[i].RefreshRate.Numerator;
				Denominator = DisplayModeList[i].RefreshRate.Denominator;
				break;
			}
		}
	}

	// 어댑터(그래픽카드)의 Description을 가져옴.
	Result = Adapter->GetDesc(&AdapterDesc);
	if (FAILED(Result))
	{
		return false;
	}

	// 현재 그래픽카드의 메모리 용량을 메가바이트 단위로 저장.
	mVideoCardMemory = static_cast<int>(AdapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 그래픽카드의 이름을 char형 문자열 배열로 바꾼 뒤 저장.
	Error = wcstombs_s(&StringLength, mVideoCardDescription, 128, AdapterDesc.Description, 128);
	if (Error != 0)
	{
		return false;
	}

	// 디스플레이 모드 리스트의 할당을 해제합니다.
	delete[] DisplayModeList;
	DisplayModeList = nullptr;

	// 출력 어댑터를 할당 해제합니다.
	AdapterOutput->Release();
	AdapterOutput = nullptr;

	// 어댑터를 할당 해제합니다.
	Adapter->Release();
	Adapter = nullptr;

	// 팩토리 객체를 할당 해제합니다.
	Factory->Release();
	Factory = nullptr;


	// 스왑체인 Description을 초기화함.
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	// 하나의 백버퍼만을 사용하도록 함.
	SwapChainDesc.BufferCount = 1;

	// 백버퍼의 너비와 높이를 설정함.
	SwapChainDesc.BufferDesc.Width = ScreenWidth;
	SwapChainDesc.BufferDesc.Height = ScreenHeight;

	// 백버퍼로 일반적인 32bit의 서페이스를 지정함.
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 백버퍼의 새로고침 비율을 설정함.
	if (mVSyncEnabled == true)
	{
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = Numerator;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = Denominator;
	}
	else
	{
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// 백버퍼의 용도를 설정함.
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 렌더링이 이루어질 윈도우의 핸들을 설정함.
	SwapChainDesc.OutputWindow = hWnd;

	// 멀티샘플링을 끔.
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;

	// 윈도우 모드 또는 풀스크린 모드를 설정함.
	if (FullScreen == true)
	{
		SwapChainDesc.Windowed = false;
	}
	else
	{
		SwapChainDesc.Windowed = true;
	}

	// 스캔라인의 정렬과 스캔라이닝을 지정되지 않음으로 설정.
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 출력 된 이후의 백버퍼의 내용을 버리도록 함.
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가옵션 플래그를 사용하지 않음.
	SwapChainDesc.Flags = 0;

	// 피쳐 레벨(사용 DirectX Version)을 DirectX 11로 설정함.
	FeatureLevel = D3D_FEATURE_LEVEL_11_0;


	// 스왑체인, Direct3D 디바이스, Direct3D 디바이스 컨텍스트를 생성함.
	Result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &FeatureLevel, 1,
		D3D11_SDK_VERSION, &SwapChainDesc, &mSwapChain, &mDevice, nullptr, &mDeviceContext);
	if (FAILED(Result))
	{
		return false;
	}

	// 백버퍼의 포인터를 가져옴.
	Result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&BackBufferPtr));
	if (FAILED(Result))
	{
		return false;
	}

	// 백버퍼의 포인터로 렌더타겟 뷰를 생성함.
	Result = mDevice->CreateRenderTargetView(BackBufferPtr, nullptr, &mRenderTargetView);
	if (FAILED(Result))
	{
		return false;
	}

	// 백버퍼 포인터를 더 이상 사용하지 않으므로 할당해제.
	BackBufferPtr->Release();
	BackBufferPtr = nullptr;

	// 깊이 버퍼의 Description을 초기화함.
	ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));

	//깊이 버퍼의 Description을 작성함.
	DepthBufferDesc.Width = ScreenWidth;
	DepthBufferDesc.Height = ScreenHeight;
	DepthBufferDesc.MipLevels = 1;
	DepthBufferDesc.ArraySize = 1;
	DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthBufferDesc.SampleDesc.Count = 1;
	DepthBufferDesc.SampleDesc.Quality = 0;
	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthBufferDesc.CPUAccessFlags = 0;
	DepthBufferDesc.MiscFlags = 0;

	// Description을 사용하여 깊이 버퍼의 텍스쳐를 생성함.
	Result = mDevice->CreateTexture2D(&DepthBufferDesc, nullptr, &mDepthStencilBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// 스텐실 상태의 Description을 초기화함.
	ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));

	// 스텐실 상태의 Description을 작성함.
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	// 픽셀이 앞면일 경우의 스텐실 작업.
	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 픽셀이 뒷면일 경우의 스텐실 작업.
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이 - 스텐실 상태를 생성함.
	Result = mDevice->CreateDepthStencilState(&DepthStencilDesc, &mDepthStencilState);
	if (FAILED(Result))
	{
		return false;
	}

	// 깊이 - 스텐실 상태를 설정함.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	// 깊이 - 스텐실 뷰의 Description을 초기화함.
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));

	// 깊이 - 스텐실 뷰의 Description을 작성함.
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이 - 스텐실 뷰를 생성.
	Result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &DepthStencilViewDesc, &mDepthStencilView);
	if (FAILED(Result))
	{
		return false;
	}

	// 렌더타겟 뷰와 깊이 - 스텐실 버퍼를 각각 출력 파이프라인에 바인딩.
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// 여기까지의 기능은 출력에 있어 필수적인 기능임.

	// 어떤 도형을 어떻게 그릴 것인지 결정하는 래스터화기 Description을 작성.
	RasterDesc.AntialiasedLineEnable = false;
	RasterDesc.CullMode = D3D11_CULL_NONE; // 임시로 BACK을 Culling하지않도록 설정.
	RasterDesc.DepthBias = 0;
	RasterDesc.DepthBiasClamp = 0.0f;
	RasterDesc.DepthClipEnable = true;
	RasterDesc.FillMode = D3D11_FILL_SOLID;
	RasterDesc.FrontCounterClockwise = false;
	RasterDesc.MultisampleEnable = false;
	RasterDesc.ScissorEnable = false;
	RasterDesc.SlopeScaledDepthBias = 0.0f;

	// 작성한 Description으로부터 래스터화기 상태를 생성함.
	Result = mDevice->CreateRasterizerState(&RasterDesc, &mRasterState);
	if (FAILED(Result))
	{
		return false;
	}

	// 래스터화기 상태를 설정함.
	mDeviceContext->RSSetState(mRasterState);


	// 렌더링을 위한 뷰포트를 설정함.
	ViewPort.Width = static_cast<float>(ScreenWidth);
	ViewPort.Height = static_cast<float>(ScreenHeight);
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	ViewPort.TopLeftX = 0.0f;
	ViewPort.TopLeftY = 0.0f;

	// 뷰포트를 생성합니다.
	mDeviceContext->RSSetViewports(1, &ViewPort);

	// 투영행렬을 설정함.
	FieldOfView = static_cast<float>(DirectX::XM_PI / 4.0f);
	ScreenAspect = static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight);

	// 3D 렌더링을 위한 투영행렬을 생성함.
	DirectX::XMStoreFloat4x4(&mProjectionMatrix, DirectX::XMMatrixPerspectiveFovLH(FieldOfView, ScreenAspect, ScreenNear, ScreenDepth));

	// 월드 행렬을 단위 행렬로 초기화.
	DirectX::XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixIdentity());

	// 2D 렌더링에 사용 될 직교 투영 행렬을 생성함.
	DirectX::XMStoreFloat4x4(&mOrthoMatrix, DirectX::XMMatrixOrthographicLH(static_cast<float>(ScreenWidth), static_cast<float>(ScreenHeight), ScreenNear, ScreenDepth));

	// 두번째 깊이-스텐실 상태변수를 초기화.
	ZeroMemory(&DepthDisableStencilDesc, sizeof(DepthDisableStencilDesc));

	// 2D 렌더링을 위한 Z버퍼가 꺼진 두번째 깊이-스텐실 상태를 생성.
	// DepthEnalble이 false인 것을 제외하면 모두 동일함.
	DepthDisableStencilDesc.DepthEnable = false;
	DepthDisableStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthDisableStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	DepthDisableStencilDesc.StencilEnable = true;
	DepthDisableStencilDesc.StencilReadMask = 0xFF;
	DepthDisableStencilDesc.StencilWriteMask = 0xFF;
	DepthDisableStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthDisableStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthDisableStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthDisableStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	DepthDisableStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthDisableStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthDisableStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthDisableStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이-스텐실 상태를 생성.
	Result = mDevice->CreateDepthStencilState(&DepthDisableStencilDesc, &mDepthDisabledStencilState);
	if (FAILED(Result))
	{
		return false;
	}

	return true;
}

void D3DClass::Shutdown()
{
	// 종료하기 전에 이렇게 윈도우 모드로 바꾸지 않으면 스왑체인을 할당 해제 할때 예외가 발생함.
	if (mSwapChain != nullptr)
	{
		mSwapChain->SetFullscreenState(false, nullptr);
	}

	if (mDepthDisabledStencilState != nullptr)
	{
		mDepthDisabledStencilState->Release();
		mDepthDisabledStencilState = nullptr;
	}

	if (mRasterState != nullptr)
	{
		mRasterState->Release();
		mRasterState = nullptr;
	}

	if (mDepthStencilView != nullptr)
	{
		mDepthStencilView->Release();
		mDepthStencilView = nullptr;
	}

	if (mDepthStencilState != nullptr)
	{
		mDepthStencilState->Release();
		mDepthStencilState = nullptr;
	}

	if (mDepthStencilBuffer != nullptr)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = nullptr;
	}

	if (mRenderTargetView != nullptr)
	{
		mRenderTargetView->Release();
		mRenderTargetView = nullptr;
	}

	if (mDevice != nullptr)
	{
		mDevice->Release();
		mDevice = nullptr;
	}

	if (mSwapChain != nullptr)
	{
		mSwapChain->Release();
		mSwapChain = nullptr;
	}

	return;
}

void D3DClass::BeginScene(float Red, float Green, float Blue, float Alpha)
{
	float Color[4];

	// 버퍼를 어떤 색상으로 지울 것인지 결정.
	Color[0] = Red;
	Color[1] = Green;
	Color[2] = Blue;
	Color[3] = Alpha;

	// 백버퍼의 내용을 지움.
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, Color);

	// 깊이버퍼를 지움.
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene()
{
	// 렌더링이 완료되었으므로 백버퍼의 내용을 화면에 표시.
	if (mVSyncEnabled == true)
	{
		// 새로고침 배율을 고정함.
		mSwapChain->Present(1, 0);
	}
	else
	{
		mSwapChain->Present(0, 0);
	}

	return;
}

ID3D11Device* D3DClass::GetDevice()
{
	return mDevice;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return mDeviceContext;
}

DirectX::XMMATRIX D3DClass::GetProjectionMatrix() const
{
	return DirectX::XMLoadFloat4x4(&mProjectionMatrix);
}

DirectX::XMMATRIX D3DClass::GetWorldMatrix() const
{
	return DirectX::XMLoadFloat4x4(&mWorldMatrix);
}

DirectX::XMMATRIX D3DClass::GetOrthoMatrix() const
{
	return DirectX::XMLoadFloat4x4(&mOrthoMatrix);
}

void D3DClass::GetVideoCardInfo(char* CardName, int& Memory)
{
	strcpy_s(CardName, 128, mVideoCardDescription);
	Memory = mVideoCardMemory;
	return;
}

void D3DClass::TurnZBufferOn()
{
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);
	return;
}

void D3DClass::TurnZBufferOff()
{
	mDeviceContext->OMSetDepthStencilState(mDepthDisabledStencilState, 1);
	return;
}

