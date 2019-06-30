
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

	// �Լ��� ���̿� �ʺ� �����ϱ� ���� 0���� �ʱ�ȭ�Ѵ�.
	ScreenWidth = 0;
	ScreenHeight = 0;

	// Windows API�� ����Ͽ� �ʱ�ȭ�Ѵ�.
	InitializeWindows(ScreenWidth, ScreenHeight);

	// Input ��ü�� ����. �� ��ü�� ������ ��� Ű���� �Է��� ó����.
	mInput = new InputClass;
	if (mInput == nullptr)
	{
		return false;
	}

	// Input ��ü�� �ʱ�ȭ�մϴ�.
	mInput->Initialize();

	// Graphics ��ü�� ����. �� ��ü�� ���ø����̼��� ��� �׷��� ��Ҹ� �׸�.
	mGraphics = new GraphicsClass;
	if (mGraphics == nullptr)
	{
		return false;
	}

	// Graphics ��ü�� �ʱ�ȭ
	Result = mGraphics->Initialize(ScreenWidth, ScreenHeight, mhWnd);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	// Graphics ��ü�� �޸𸮸� ����.
	if (mGraphics != nullptr)
	{
		mGraphics->Shutdown();
		delete mGraphics;
		mGraphics = nullptr;
	}

	// Input ��ü�� �޸𸮸� ����.
	if (mInput != nullptr)
	{
		delete mInput;
		mInput = nullptr;
	}

	// â�� ������.
	ShutdownWindows();

	return;
}

/*
Run�Լ� �ǻ��ڵ�

1. �������� �ý��� �޽����� Ȯ��
2. �޼��� ó��
3. ���ø����̼��� �۾�
4. ������ �۾� �� ���α׷��� ���Ḧ ���ϴ��� Ȯ��
*/
void SystemClass::Run()
{
	MSG Msg;
	bool Done;
	bool Result;

	// �޼��� ����ü�� �޸𸮿��� �ʱ�ȭ�մϴ�.
	ZeroMemory(&Msg, sizeof(MSG));

	// �����κ��� ���� �޼����� ���� ������ ������ ���ϴ�.
	Done = false;
	while (Done == false)
	{
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		// �����쿡�� ���ø����̼��� ���Ḧ ��û�ϴ� ��� ��������.
		if (Msg.message == WM_QUIT)
		{
			Done = true;
		}
		else // �� �ܿ��� Frame �Լ��� ó���մϴ�.
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

	// ������ ESCŰ�� ���� ���ø����̼��� ���Ḧ ���ϴ��� Ȯ����.
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
		// Ű���尡 ���ȴ��� Ȯ��.
		case WM_KEYDOWN:
		{
			// Ű�� ���ȴٸ� Input ��ü�� �� ������ ����.
			mInput->KeyDown(static_cast<UINT>(wParam));
			return 0;
		}

		// Ű������ ���� Ű�� ���������� Ȯ��.
		case WM_KEYUP:
		{
			// Ű�� �������ٸ� Input ��ü�� �� ������ ����.
			mInput->KeyUp(static_cast<UINT>(wParam));
			return 0;
		}

		// ���� �ٸ� �޼����� ������� �����Ƿ� �⺻ �޼��� ó����� ����.
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

	// �ܺ� �����͸� �� ��ü�� ����.
	ApplicationHandle = this;

	// �� ���ø����̼��� �ν��Ͻ��� ������.
	mhInstance = GetModuleHandle(NULL);

	// ���ø����̼��� �̸��� ������.
	mApplicationName = "Engine";

	// ������ Ŭ������ �⺻ �������� ����ϴ�.
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

	// ������ Ŭ������ ���.
	RegisterClassEx(&WC);

	// ����� ȭ���� �ػ󵵸� �˾ƿɴϴ�.
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Ǯ��ũ�� ��� ������ ���� ���� ȭ�� ����.
	if (FULL_SCREEN == true)
	{
		// ����, Ǯ��ũ�� ����� ȭ��ũ�⸦ ����ũ�� ũ�⿡ ���߰� ������ 32bit�� ����.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= static_cast<UINT>(ScreenWidth);
		dmScreenSettings.dmPelsHeight	= static_cast<UINT>(ScreenHeight);
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ���� �´� ���÷��� ����.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// �������� ��ġ�� ȭ�鿡 ���� ���� ����.
		PosX = PosY = 0;
	}
	else
	{
		// ������ ����� 800*600�� ȭ�� ũ�⸦ ����.
		ScreenWidth	= 800;
		ScreenHeight = 600;

		// â�� ������� �߾ӿ� ������ ����.
		PosX = (GetSystemMetrics(SM_CXSCREEN) - ScreenWidth) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - ScreenHeight) / 2;
	}

	// ������ ���� ������ â�� ����� �� �ڵ��� ������.
	mhWnd = CreateWindowEx(WS_EX_APPWINDOW, mApplicationName, mApplicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		PosX, PosY, ScreenWidth, ScreenHeight, nullptr, nullptr, mhInstance, nullptr);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ��.
	ShowWindow(mhWnd, SW_SHOW);
	SetForegroundWindow(mhWnd);
	SetFocus(mhWnd);

	// ���콺 Ŀ���� ǥ������ ����.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// ���콺 Ŀ���� ǥ����.
	ShowCursor(true);

	// Ǯ��ũ�� ��带 �������� �� ���÷��� ������ ����.
	if (FULL_SCREEN == true)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	// â�� ����.
	DestroyWindow(mhWnd);
	mhWnd = nullptr;

	// ���ø����̼� �ν��Ͻ��� ����.
	UnregisterClass(mApplicationName, mhInstance);
	mhInstance = nullptr;

	// �� Ŭ������ ���� �ܺ� ������ ������ ����.
	ApplicationHandle = nullptr;

	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT UMessage, WPARAM wParam, LPARAM lParam)
{
	switch (UMessage)
	{
		// �����찡 ���ŵǾ����� Ȯ��.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// �����찡 �������� Ȯ��.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// �ٸ� ��� �޼������� System Ŭ������ �޼��� ó���⿡ ����.
		default:
		{
			return ApplicationHandle->MessageHandler(hWnd, UMessage, wParam, lParam);
		}
	}
}

