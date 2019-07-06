
#include "GraphicsClass.h"


GraphicsClass::GraphicsClass()
{
	mD3D = nullptr;
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

	//Direct3D 객체를 생성.
	mD3D = new D3DClass;
	if (mD3D == nullptr)
	{
		return false;
	}

	//Direct3D 객체를 초기화.
	Result = mD3D->Initialize(ScreenWidth, ScreenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (Result == false)
	{
		MessageBox(hWnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
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

	// 그래픽 렌더링을 수행.
	Result = Render();
	if (Result == false)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	// 씬 그리기를 시작하기 위해 버퍼의 내용을 지움.
	mD3D->BeginScene(1.0f, 1.0f, 0.0f, 1.0f);

	// 버퍼에 그려진 씬을 화면에 표시함.
	mD3D->EndScene();

	return true;
}

