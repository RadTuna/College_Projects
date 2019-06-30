
#include "SystemClass.h"

SystemClass::SystemClass()
{
	mInput = nullptr;
	mGraphics = nullptr;
}

SystemClass::SystemClass(const SystemClass& Other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int ScreenWidth;
	int ScreenHeight;
	bool Result;

	// 함수에 높이와 너비를 전달하기 위해 0으로 초기화한다.
	ScreenWidth = 0;
	ScreenHeight = 0;

	// Windows API를 사용하여 초기화한다.
	InitializeWindows(ScreenWidth, ScreenHeight);

	// Input 객체를 생성. 이 객체는 유저의 모든 키보드 입력을 처리함.
	mInput = new InputClass;
	if (mInput == nullptr)
	{
		return false;
	}

	// Input 객체를 초기화합니다.
	mInput->Initialize();

	// Graphics 객체를 생성. 이 객체는 어플리케이션의 모든 그래픽 요소를 그림.
	mGraphics = new GraphicsClass;
	if (mGraphics == nullptr)
	{
		return false;
	}

	// Graphics 객체를 초기화
	Result = mGraphics->Initialize(ScreenWidth, ScreenHeight, mhWnd);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	// Graphics 객체의 메모리를 해제.
	if (mGraphics != nullptr)
	{
		mGraphics->Shutdown();
		delete mGraphics;
		mGraphics = nullptr;
	}

	// Input 객체의 메모리를 해제.
	if (mInput != nullptr)
	{
		delete mInput;
		mInput = nullptr;
	}

	// 창을 종료함.
	ShutdownWindows();

	return;
}

/*
Run함수 의사코드

1. 윈도우의 시스템 메시지를 확인
2. 메세지 처리
3. 어플리케이션의 작업
4. 유저가 작업 중 프로그램의 종료를 원하는지 확인
*/
void SystemClass::Run()
{
	MSG Msg;
	bool Done;
	bool Result;

	// 메세지 구조체를 메모리에서 초기화합니다.
	ZeroMemory(&Msg, sizeof(MSG));

	// 유저로부터 종료 메세지를 받을 때까지 루프를 돕니다.
	Done = false;
	while (Done == false)
	{
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		// 윈도우에서 어플리케이션의 종료를 요청하는 경우 빠져나감.
		if (Msg.message == WM_QUIT)
		{
			Done = true;
		}
		else // 그 외에는 Frame 함수를 처리합니다.
		{
			Result = Frame();
			if (Result == false)
			{
				Done = true;
			}
		}
	}

	return;
}

bool SystemClass::Frame()
{
	bool Result;

	// 유저가 ESC키를 눌러 어플리케이션을 종료를 원하는지 확인함.
	if (mInput->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	Result = mGraphics->Frame();
	if (Result == false)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT UMsg, WPARAM wParam, LPARAM lParam)
{
	switch (UMsg)
	{
		// 키보드가 눌렸는지 확인.
		case WM_KEYDOWN:
		{
			// 키가 눌렸다면 Input 객체에 이 정보를 전달.
			mInput->KeyDown(static_cast<UINT>(wParam));
			return 0;
		}

		// 키보드의 눌린 키가 떼어졌는지 확인.
		case WM_KEYUP:
		{
			// 키가 떼어졌다면 Input 객체에 이 정보를 전달.
			mInput->KeyUp(static_cast<UINT>(wParam));
			return 0;
		}

		// 들어온 다른 메세지는 사용하지 않으므로 기본 메세지 처리기로 전달.
		default:
		{
			return DefWindowProc(hWnd, UMsg, wParam, lParam);
		}
	}
}

void SystemClass::InitializeWindows(int& ScreenWidth, int& ScreenHeight)
{
	WNDCLASSEX WC;
	DEVMODE dmScreenSettings;
	int PosX;
	int PosY;

	// 외부 포인터를 이 객체로 설정.
	ApplicationHandle = this;

	// 이 어플리케이션의 인스턴스를 가져옴.
	mhInstance = GetModuleHandle(NULL);

	// 어플리케이션의 이름을 설정함.
	mApplicationName = "Engine";

	// 윈도우 클래스를 기본 설정으로 맞춥니다.
	WC.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WC.lpfnWndProc		= WndProc;
	WC.cbClsExtra		= 0;
	WC.cbWndExtra		= 0;
	WC.hInstance		= mhInstance;
	WC.hIcon			= LoadIcon(nullptr, IDI_WINLOGO);
	WC.hIconSm			= WC.hIcon;
	WC.hCursor			= LoadCursor(nullptr, IDC_ARROW);
	WC.hbrBackground	= static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	WC.lpszMenuName		= nullptr;
	WC.lpszClassName	= mApplicationName;
	WC.cbSize			= sizeof(WNDCLASSEX);

	// 윈도우 클래스를 등록.
	RegisterClassEx(&WC);

	// 모니터 화면의 해상도를 알아옵니다.
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 풀스크린 모드 변수의 값에 따라 화면 설정.
	if (FULL_SCREEN == true)
	{
		// 만약, 풀스크린 모드라면 화면크기를 데스크톱 크기에 맞추고 색상을 32bit로 조정.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= static_cast<UINT>(ScreenWidth);
		dmScreenSettings.dmPelsHeight	= static_cast<UINT>(ScreenHeight);
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 풀스크린에 맞는 디스플레이 설정.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 윈도우의 위치를 화면에 왼쪽 위로 맞춤.
		PosX = PosY = 0;
	}
	else
	{
		// 윈도우 모드라면 800*600의 화면 크기를 가짐.
		ScreenWidth	= 800;
		ScreenHeight = 600;

		// 창을 모니터의 중앙에 오도록 조정.
		PosX = (GetSystemMetrics(SM_CXSCREEN) - ScreenWidth) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - ScreenHeight) / 2;
	}

	// 설정한 것을 가지고 창을 만들고 그 핸들을 가져옴.
	mhWnd = CreateWindowEx(WS_EX_APPWINDOW, mApplicationName, mApplicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		PosX, PosY, ScreenWidth, ScreenHeight, nullptr, nullptr, mhInstance, nullptr);

	// 윈도우를 화면에 표시하고 포커스.
	ShowWindow(mhWnd, SW_SHOW);
	SetForegroundWindow(mhWnd);
	SetFocus(mhWnd);

	// 마우스 커서를 표시하지 않음.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// 마우스 커서를 표시함.
	ShowCursor(true);

	// 풀스크린 모드를 빠져나올 때 디스플레이 설정을 변경.
	if (FULL_SCREEN == true)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	// 창을 제거.
	DestroyWindow(mhWnd);
	mhWnd = nullptr;

	// 어플리케이션 인스턴스를 제거.
	UnregisterClass(mApplicationName, mhInstance);
	mhInstance = nullptr;

	// 이 클래스에 대한 외부 포인터 참조를 제거.
	ApplicationHandle = nullptr;

	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT UMessage, WPARAM wParam, LPARAM lParam)
{
	switch (UMessage)
	{
		// 윈도우가 제거되었는지 확인.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// 윈도우가 닫히는지 확인.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// 다른 모든 메세지들은 System 클래스의 메세지 처리기에 전달.
		default:
		{
			return ApplicationHandle->MessageHandler(hWnd, UMessage, wParam, lParam);
		}
	}
}

