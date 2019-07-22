
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

	// Direct3D 객체를 생성.
	mD3D = new D3DClass;
	if (mD3D == nullptr)
	{
		return false;
	}

	// Direct3D 객체를 초기화.
	Result = mD3D->Initialize(ScreenWidth, ScreenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	// Camera 객체를 생성.
	mCamera = new CameraClass;
	if (mCamera == nullptr)
	{
		return false;
	}

	// Camera의 초기위치를 설정.
	mCamera->SetPosition(0.0f, 5.0f, -8.0f);
	mCamera->SetRotation(30.0f, 0.0f, 0.0f);
	
	// Model 객체를 생성.
	mModel = new ModelClass;
	if (mModel == nullptr)
	{
		return false;
	}

	// Model 객체를 초기화.
	Result = mModel->Initialize(mD3D->GetDevice(), mD3D->GetDeviceContext(), "../DirectXTutorials/Data/stone01.tga", "../DirectXTutorials/Data/cube.txt", false);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize Model object", "Error", MB_OK);
		return false;
	}

	// LightShader 객체를 생성.
	mLightShader = new LightShaderClass;
	if (mLightShader == nullptr)
	{
		return false;
	}

	// LightShader 객체를 초기화.
	Result = mLightShader->Initialize(mD3D->GetDevice(), hWnd);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize ColorShader object", "Error", MB_OK);
		return false;
	}

	// Light 객체를 생성.
	mLight = new LightClass;
	if (mLight == nullptr)
	{
		return false;
	}

	// Light 객체를 초기화.
	mLight->SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	mLight->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	mLight->SetDirection(0.0f, -1.0f, 1.0f);
	mLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	mLight->SetSpecularPower(1.0f);

	return true;
}

void GraphicsClass::Shutdown()
{
	// Light 객체를 반환.
	if (mLight != nullptr)
	{
		delete mLight;
		mLight = nullptr;
	}

	// LightShader 객체를 반환.
	if (mLightShader != nullptr)
	{
		mLightShader->Shutdown();
		delete mLightShader;
		mLightShader = nullptr;
	}

	// Model 객체를 반환.
	if (mModel != nullptr)
	{
		mModel->Shutdown();
		delete mModel;
		mModel = nullptr;
	}

	// Camera 객체를 반환.
	if (mCamera != nullptr)
	{
		delete mCamera;
		mCamera = nullptr;
	}

	// D3D 객체를 반환.
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

	// 매 프레임 마다 Rotation변수를 수정.
	Rotation += static_cast<float>(DirectX::XM_PI * 0.005f);
	if (Rotation > 360.0f)
	{
		Rotation -= 360.0f;
	}

	// 그래픽 렌더링을 수행.
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

	// 씬 그리기를 시작하기 위해 버퍼의 내용을 지움.
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치를 기반으로 View행렬을 생성함.
	mCamera->Render();

	// 카메라, D3D 객체로부터 행렬들을 획득.
	ViewMat = mCamera->GetViewMatrix();
	WorldMat = mD3D->GetWorldMatrix();
	ProjectionMat = mD3D->GetProjectionMatrix();

	// 모델이 회전하도록 Rotation변수를 이용해 월드 행렬을 회전함.
	WorldMat = DirectX::XMMatrixRotationY(Rotation);

	// 그래픽 파이프라인이 그리는 것을 준비하기 위해 버텍스-인덱스 버퍼를 넣음.
	mModel->Render(mD3D->GetDeviceContext());

	// TextureShader를 사용해 모델을 렌더링.
	Result = mLightShader->Render(mD3D->GetDeviceContext(), mModel->GetIndexCount(), 
		WorldMat, ViewMat, ProjectionMat, mModel->GetTexture(), mLight->GetDirection(), mLight->GetDiffuseColor(), mLight->GetAmbientColor(),
		mCamera->GetPostion(), mLight->GetSpecularColor(), mLight->GetSpecularPower());
	if (Result == false)
	{
		return false;
	}

	// 버퍼에 그려진 씬을 화면에 표시함.
	mD3D->EndScene();

	return true;
}

