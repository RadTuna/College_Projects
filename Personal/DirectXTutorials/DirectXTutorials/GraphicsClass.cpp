
#include "GraphicsClass.h"


GraphicsClass::GraphicsClass()
{
	mD3D = nullptr;
	mCamera = nullptr;
	mModel = nullptr;
	mLightShader = nullptr;
	mLight = nullptr;
	mBitmap = nullptr;
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
	mCamera->SetPosition(0.0f, 0.0f, -10.0f);
	mCamera->SetRotation(0.0f, 0.0f, 0.0f);
	
	// Model ��ü�� ����.
	mModel = new ModelClass;
	if (mModel == nullptr)
	{
		return false;
	}

	// Model ��ü�� �ʱ�ȭ // IsFBX = true : fbx����Է� // IsFBX = false : txt����Է�
	Result = mModel->Initialize(mD3D->GetDevice(), mD3D->GetDeviceContext(), "../DirectXTutorials/Data/stone01.tga", "../DirectXTutorials/Data/teapot.fbx", true);
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
		MessageBox(hWnd, "Could not initialize LightShader object", "Error", MB_OK);
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
	mLight->SetDiffuseColor(0.6f, 0.6f, 0.6f, 1.0f);
	mLight->SetDirection(0.0f, -1.0f, 1.0f);
	mLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	mLight->SetSpecularPower(0.6f);

	// ��Ʈ�� ��ü�� ����.
	mBitmap = new BitmapClass;
	if (mBitmap == nullptr)
	{
		return false;
	}

	// ��Ʈ�� ��ü�� �ʱ�ȭ.
	Result = mBitmap->Initialize(mD3D->GetDevice(), mD3D->GetDeviceContext(), ScreenWidth, ScreenHeight, "../DirectXTutorials/Data/stone01.tga", 256, 256);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize Bitmap object", "Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// ��Ʈ�� ��ü�� ��ȯ.
	if (mBitmap != nullptr)
	{
		mBitmap->Shutdown();
		delete mBitmap;
		mBitmap = nullptr;
	}

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
	DirectX::XMMATRIX OrthoMat;

	// �� �׸��⸦ �����ϱ� ���� ������ ������ ����.
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ������� View����� ������.
	mCamera->Render();

	// ī�޶�, D3D ��ü�κ��� ��ĵ��� ȹ��.
	ViewMat = mCamera->GetViewMatrix();
	WorldMat = mD3D->GetWorldMatrix();
	ProjectionMat = mD3D->GetProjectionMatrix();
	OrthoMat = mD3D->GetOrthoMatrix();

	// 2D �������� �����ϱ� ������ Z���۸� ��.
	mD3D->TurnZBufferOff();

	// ��Ʈ�� ���ؽ��� �ε��� ���۸� �׷��� ���������ο� ��ġ�ϸ� ��ο츦 �غ�.
	Result = mBitmap->Render(mD3D->GetDeviceContext(), 200, -200);
	if (Result == false)
	{
		return false;
	}

	// LightShader�� ����� ��Ʈ���� ������.
	Result = mLightShader->Render(mD3D->GetDeviceContext(), mBitmap->GetIndexCount(), WorldMat, ViewMat, OrthoMat, mBitmap->GetTexture(),
		DirectX::XMVectorZero(), DirectX::XMVectorZero(), DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMVectorZero(), DirectX::XMVectorZero(), 1.0f);
	if (Result == false)
	{
		return false;
	}

	// 2D �������� ���� �Ǿ����� Z���۸� ��.
	mD3D->TurnZBufferOn();

	// ���� ȸ���ϵ��� Rotation������ �̿��� ���� ����� ȸ����.
	WorldMat = DirectX::XMMatrixRotationY(Rotation);

	// �׷��� ������������ �׸��� ���� �غ��ϱ� ���� ���ؽ�-�ε��� ���۸� ����.
	mModel->Render(mD3D->GetDeviceContext());

	// LightShader�� ����� ���� ������.
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

