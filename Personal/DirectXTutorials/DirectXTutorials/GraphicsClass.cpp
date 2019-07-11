
#include "GraphicsClass.h"


GraphicsClass::GraphicsClass()
{
	mD3D = nullptr;
	mCamera = nullptr;
	mModel = nullptr;
	mColorShader = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int ScreenWidth, int ScreenHeight, HWND hWnd)
{
	bool Result;

	// Direct3D ��ü�� ����.
	mD3D = new D3DClass;
	if (mD3D == nullptr)
	{
		return false;
	}

	// Direct3D ��ü�� �ʱ�ȭ.
	Result = mD3D->Initialize(ScreenWidth, ScreenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	// Camera ��ü�� ����.
	mCamera = new CameraClass;
	if (mCamera == nullptr)
	{
		return false;
	}

	// Camera�� �ʱ���ġ�� ����.
	mCamera->SetPosition(0.0f, 0.0f, 0.0f);
	
	// Model ��ü�� ����.
	mModel = new ModelClass;
	if (mModel == nullptr)
	{
		return false;
	}

	// Model ��ü�� �ʱ�ȭ.
	Result = mModel->Initialize(mD3D->GetDevice());
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize Model object", "Error", MB_OK);
		return false;
	}

	// ColorShader ��ü�� ����.
	mColorShader = new ColorShaderClass;
	if (mColorShader == nullptr)
	{
		return false;
	}

	// ColorShader ��ü�� �ʱ�ȭ.
	Result = mColorShader->Initialize(mD3D->GetDevice(), hWnd);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize ColorShader object", "Error", MB_OK);
		return false;
	}


	return true;
}

void GraphicsClass::Shutdown()
{
	// ColorShader ��ü�� ��ȯ.
	if (mColorShader != nullptr)
	{
		mColorShader->Shutdown();
		mColorShader = nullptr;
	}

	// Model ��ü�� ��ȯ.
	if (mModel != nullptr)
	{
		mModel->Shutdown();
		mModel = nullptr;
	}

	// Camera ��ü�� ��ȯ.
	if (mCamera != nullptr)
	{
		delete mCamera;
		mCamera = nullptr;
	}

	// D3D ��ü�� ��ȯ.
	if (mD3D != nullptr)
	{
		mD3D->Shutdown();
		delete mD3D;
		mD3D = nullptr;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool Result;

	// �׷��� �������� ����.
	Result = Render();
	if (Result == false)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	bool Result;

	DirectX::XMMATRIX WorldMat;
	DirectX::XMMATRIX ViewMat;
	DirectX::XMMATRIX ProjectionMat;

	// �� �׸��⸦ �����ϱ� ���� ������ ������ ����.
	mD3D->BeginScene(0.2f, 0.0f, 0.1f, 1.0f);

	// ī�޶��� ��ġ�� ������� View����� ������.
	mCamera->Render();

	// ī�޶�, D3D ��ü�κ��� ��ĵ��� ȹ��.
	ViewMat = mCamera->GetViewMatrix();
	WorldMat = mD3D->GetWorldMatrix();
	ProjectionMat = mD3D->GetProjectionMatrix();

	// �׷��� ������������ �׸��� ���� �غ��ϱ� ���� ���ؽ�-�ε��� ���۸� ����.
	mModel->Render(mD3D->GetDeviceContext());

	// ColorShader�� ����� ���� ������.
	Result = mColorShader->Render(mD3D->GetDeviceContext(), mModel->GetIndexCount(), WorldMat, ViewMat, ProjectionMat);
	if (Result == false)
	{
		return false;
	}

	// ���ۿ� �׷��� ���� ȭ�鿡 ǥ����.
	mD3D->EndScene();

	return true;
}

