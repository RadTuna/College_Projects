
#include "GraphicsClass.h"


GraphicsClass::GraphicsClass()
{
	mD3D = nullptr;
	mCamera = nullptr;
	mModel = nullptr;
	mLightShader = nullptr;
	mLight = nullptr;
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
	mCamera->SetPosition(0.0f, 5.0f, -8.0f);
	mCamera->SetRotation(30.0f, 0.0f, 0.0f);
	
	// Model ��ü�� ����.
	mModel = new ModelClass;
	if (mModel == nullptr)
	{
		return false;
	}

	// Model ��ü�� �ʱ�ȭ.
	Result = mModel->Initialize(mD3D->GetDevice(), mD3D->GetDeviceContext(), "../DirectXTutorials/Data/stone01.tga", "../DirectXTutorials/Data/cube.txt", false);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize Model object", "Error", MB_OK);
		return false;
	}

	// LightShader ��ü�� ����.
	mLightShader = new LightShaderClass;
	if (mLightShader == nullptr)
	{
		return false;
	}

	// LightShader ��ü�� �ʱ�ȭ.
	Result = mLightShader->Initialize(mD3D->GetDevice(), hWnd);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize ColorShader object", "Error", MB_OK);
		return false;
	}

	// Light ��ü�� ����.
	mLight = new LightClass;
	if (mLight == nullptr)
	{
		return false;
	}

	// Light ��ü�� �ʱ�ȭ.
	mLight->SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	mLight->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	mLight->SetDirection(0.0f, -1.0f, 1.0f);
	mLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	mLight->SetSpecularPower(1.0f);

	return true;
}

void GraphicsClass::Shutdown()
{
	// Light ��ü�� ��ȯ.
	if (mLight != nullptr)
	{
		delete mLight;
		mLight = nullptr;
	}

	// LightShader ��ü�� ��ȯ.
	if (mLightShader != nullptr)
	{
		mLightShader->Shutdown();
		delete mLightShader;
		mLightShader = nullptr;
	}

	// Model ��ü�� ��ȯ.
	if (mModel != nullptr)
	{
		mModel->Shutdown();
		delete mModel;
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
	static float Rotation = 5.0f;

	// �� ������ ���� Rotation������ ����.
	Rotation += static_cast<float>(DirectX::XM_PI * 0.005f);
	if (Rotation > 360.0f)
	{
		Rotation -= 360.0f;
	}

	// �׷��� �������� ����.
	Result = Render(Rotation);
	if (Result == false)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float Rotation)
{
	bool Result;

	DirectX::XMMATRIX WorldMat;
	DirectX::XMMATRIX ViewMat;
	DirectX::XMMATRIX ProjectionMat;

	// �� �׸��⸦ �����ϱ� ���� ������ ������ ����.
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ������� View����� ������.
	mCamera->Render();

	// ī�޶�, D3D ��ü�κ��� ��ĵ��� ȹ��.
	ViewMat = mCamera->GetViewMatrix();
	WorldMat = mD3D->GetWorldMatrix();
	ProjectionMat = mD3D->GetProjectionMatrix();

	// ���� ȸ���ϵ��� Rotation������ �̿��� ���� ����� ȸ����.
	WorldMat = DirectX::XMMatrixRotationY(Rotation);

	// �׷��� ������������ �׸��� ���� �غ��ϱ� ���� ���ؽ�-�ε��� ���۸� ����.
	mModel->Render(mD3D->GetDeviceContext());

	// TextureShader�� ����� ���� ������.
	Result = mLightShader->Render(mD3D->GetDeviceContext(), mModel->GetIndexCount(), 
		WorldMat, ViewMat, ProjectionMat, mModel->GetTexture(), mLight->GetDirection(), mLight->GetDiffuseColor(), mLight->GetAmbientColor(),
		mCamera->GetPostion(), mLight->GetSpecularColor(), mLight->GetSpecularPower());
	if (Result == false)
	{
		return false;
	}

	// ���ۿ� �׷��� ���� ȭ�鿡 ǥ����.
	mD3D->EndScene();

	return true;
}

