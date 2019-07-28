
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

// D3D11�� �ʱ�ȭ�ϴ� �Լ�.
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

	//VSync ������ ����.
	mVSyncEnabled = VSync;

	// DirectX �׷��� �������̽� ���丮�� ����.
	Result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));
	if (FAILED(Result))
	{
		return false;
	}

	// ���丮 ��ü�� ����Ͽ� ù��° �׷���ī�� �������̽��� ���� ����͸� ����.
	Result = Factory->EnumAdapters(0, &Adapter);
	if (FAILED(Result))
	{
		return false;
	}

	// ��¿� ���� ù��° ����͸� ������.
	Result = Adapter->EnumOutputs(0, &AdapterOutput);
	if (FAILED(Result))
	{
		return false;
	}

	// DXGI_FORMAT_R8G8B8A8_UNORM ����� ��� ���÷��� ���˿� �´� ����� ������ ����.
	Result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &NumModes, nullptr);
	if (FAILED(Result))
	{
		return false;
	}

	// ������ ��� ����Ϳ� �׷���ī�� ������ ������ ����Ʈ�� ����.
	DisplayModeList = new DXGI_MODE_DESC[NumModes];
	if (DisplayModeList == nullptr)
	{
		return false;
	}

	// ���÷��� ��忡 ���� ����Ʈ ������ ä������.
	Result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &NumModes, DisplayModeList);
	if (FAILED(Result))
	{
		return false;
	}

	// ���� ��� ���÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��� ��带 ã��.
	// ������ ��带 ã���� ������� ���ΰ�ħ ������ �и�� ���ڰ��� ������.
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

	// �����(�׷���ī��)�� Description�� ������.
	Result = Adapter->GetDesc(&AdapterDesc);
	if (FAILED(Result))
	{
		return false;
	}

	// ���� �׷���ī���� �޸� �뷮�� �ް�����Ʈ ������ ����.
	mVideoCardMemory = static_cast<int>(AdapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// �׷���ī���� �̸��� char�� ���ڿ� �迭�� �ٲ� �� ����.
	Error = wcstombs_s(&StringLength, mVideoCardDescription, 128, AdapterDesc.Description, 128);
	if (Error != 0)
	{
		return false;
	}

	// ���÷��� ��� ����Ʈ�� �Ҵ��� �����մϴ�.
	delete[] DisplayModeList;
	DisplayModeList = nullptr;

	// ��� ����͸� �Ҵ� �����մϴ�.
	AdapterOutput->Release();
	AdapterOutput = nullptr;

	// ����͸� �Ҵ� �����մϴ�.
	Adapter->Release();
	Adapter = nullptr;

	// ���丮 ��ü�� �Ҵ� �����մϴ�.
	Factory->Release();
	Factory = nullptr;


	// ����ü�� Description�� �ʱ�ȭ��.
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	// �ϳ��� ����۸��� ����ϵ��� ��.
	SwapChainDesc.BufferCount = 1;

	// ������� �ʺ�� ���̸� ������.
	SwapChainDesc.BufferDesc.Width = ScreenWidth;
	SwapChainDesc.BufferDesc.Height = ScreenHeight;

	// ����۷� �Ϲ����� 32bit�� �����̽��� ������.
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// ������� ���ΰ�ħ ������ ������.
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

	// ������� �뵵�� ������.
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �������� �̷���� �������� �ڵ��� ������.
	SwapChainDesc.OutputWindow = hWnd;

	// ��Ƽ���ø��� ��.
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;

	// ������ ��� �Ǵ� Ǯ��ũ�� ��带 ������.
	if (FullScreen == true)
	{
		SwapChainDesc.Windowed = false;
	}
	else
	{
		SwapChainDesc.Windowed = true;
	}

	// ��ĵ������ ���İ� ��ĵ���̴��� �������� �������� ����.
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ��� �� ������ ������� ������ �������� ��.
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰��ɼ� �÷��׸� ������� ����.
	SwapChainDesc.Flags = 0;

	// ���� ����(��� DirectX Version)�� DirectX 11�� ������.
	FeatureLevel = D3D_FEATURE_LEVEL_11_0;


	// ����ü��, Direct3D ����̽�, Direct3D ����̽� ���ؽ�Ʈ�� ������.
	Result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &FeatureLevel, 1,
		D3D11_SDK_VERSION, &SwapChainDesc, &mSwapChain, &mDevice, nullptr, &mDeviceContext);
	if (FAILED(Result))
	{
		return false;
	}

	// ������� �����͸� ������.
	Result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&BackBufferPtr));
	if (FAILED(Result))
	{
		return false;
	}

	// ������� �����ͷ� ����Ÿ�� �並 ������.
	Result = mDevice->CreateRenderTargetView(BackBufferPtr, nullptr, &mRenderTargetView);
	if (FAILED(Result))
	{
		return false;
	}

	// ����� �����͸� �� �̻� ������� �����Ƿ� �Ҵ�����.
	BackBufferPtr->Release();
	BackBufferPtr = nullptr;

	// ���� ������ Description�� �ʱ�ȭ��.
	ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));

	//���� ������ Description�� �ۼ���.
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

	// Description�� ����Ͽ� ���� ������ �ؽ��ĸ� ������.
	Result = mDevice->CreateTexture2D(&DepthBufferDesc, nullptr, &mDepthStencilBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// ���ٽ� ������ Description�� �ʱ�ȭ��.
	ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));

	// ���ٽ� ������ Description�� �ۼ���.
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	// �ȼ��� �ո��� ����� ���ٽ� �۾�.
	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �ȼ��� �޸��� ����� ���ٽ� �۾�.
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ���� - ���ٽ� ���¸� ������.
	Result = mDevice->CreateDepthStencilState(&DepthStencilDesc, &mDepthStencilState);
	if (FAILED(Result))
	{
		return false;
	}

	// ���� - ���ٽ� ���¸� ������.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	// ���� - ���ٽ� ���� Description�� �ʱ�ȭ��.
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));

	// ���� - ���ٽ� ���� Description�� �ۼ���.
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// ���� - ���ٽ� �並 ����.
	Result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &DepthStencilViewDesc, &mDepthStencilView);
	if (FAILED(Result))
	{
		return false;
	}

	// ����Ÿ�� ��� ���� - ���ٽ� ���۸� ���� ��� ���������ο� ���ε�.
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// ��������� ����� ��¿� �־� �ʼ����� �����.

	// � ������ ��� �׸� ������ �����ϴ� ������ȭ�� Description�� �ۼ�.
	RasterDesc.AntialiasedLineEnable = false;
	RasterDesc.CullMode = D3D11_CULL_NONE; // �ӽ÷� BACK�� Culling�����ʵ��� ����.
	RasterDesc.DepthBias = 0;
	RasterDesc.DepthBiasClamp = 0.0f;
	RasterDesc.DepthClipEnable = true;
	RasterDesc.FillMode = D3D11_FILL_SOLID;
	RasterDesc.FrontCounterClockwise = false;
	RasterDesc.MultisampleEnable = false;
	RasterDesc.ScissorEnable = false;
	RasterDesc.SlopeScaledDepthBias = 0.0f;

	// �ۼ��� Description���κ��� ������ȭ�� ���¸� ������.
	Result = mDevice->CreateRasterizerState(&RasterDesc, &mRasterState);
	if (FAILED(Result))
	{
		return false;
	}

	// ������ȭ�� ���¸� ������.
	mDeviceContext->RSSetState(mRasterState);


	// �������� ���� ����Ʈ�� ������.
	ViewPort.Width = static_cast<float>(ScreenWidth);
	ViewPort.Height = static_cast<float>(ScreenHeight);
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	ViewPort.TopLeftX = 0.0f;
	ViewPort.TopLeftY = 0.0f;

	// ����Ʈ�� �����մϴ�.
	mDeviceContext->RSSetViewports(1, &ViewPort);

	// ��������� ������.
	FieldOfView = static_cast<float>(DirectX::XM_PI / 4.0f);
	ScreenAspect = static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight);

	// 3D �������� ���� ��������� ������.
	DirectX::XMStoreFloat4x4(&mProjectionMatrix, DirectX::XMMatrixPerspectiveFovLH(FieldOfView, ScreenAspect, ScreenNear, ScreenDepth));

	// ���� ����� ���� ��ķ� �ʱ�ȭ.
	DirectX::XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixIdentity());

	// 2D �������� ��� �� ���� ���� ����� ������.
	DirectX::XMStoreFloat4x4(&mOrthoMatrix, DirectX::XMMatrixOrthographicLH(static_cast<float>(ScreenWidth), static_cast<float>(ScreenHeight), ScreenNear, ScreenDepth));

	// �ι�° ����-���ٽ� ���º����� �ʱ�ȭ.
	ZeroMemory(&DepthDisableStencilDesc, sizeof(DepthDisableStencilDesc));

	// 2D �������� ���� Z���۰� ���� �ι�° ����-���ٽ� ���¸� ����.
	// DepthEnalble�� false�� ���� �����ϸ� ��� ������.
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

	// ����-���ٽ� ���¸� ����.
	Result = mDevice->CreateDepthStencilState(&DepthDisableStencilDesc, &mDepthDisabledStencilState);
	if (FAILED(Result))
	{
		return false;
	}

	return true;
}

void D3DClass::Shutdown()
{
	// �����ϱ� ���� �̷��� ������ ���� �ٲ��� ������ ����ü���� �Ҵ� ���� �Ҷ� ���ܰ� �߻���.
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

	// ���۸� � �������� ���� ������ ����.
	Color[0] = Red;
	Color[1] = Green;
	Color[2] = Blue;
	Color[3] = Alpha;

	// ������� ������ ����.
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, Color);

	// ���̹��۸� ����.
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene()
{
	// �������� �Ϸ�Ǿ����Ƿ� ������� ������ ȭ�鿡 ǥ��.
	if (mVSyncEnabled == true)
	{
		// ���ΰ�ħ ������ ������.
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

