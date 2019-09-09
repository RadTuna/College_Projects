#include "CommonHeader.h"
#include "GDIManager.h"
#include "Rasterizer.h"
#include "TimeCounter.h"

#define ScreenWidth 1920
#define ScreenHeight 1080
#define WindowWidth 640
#define WindowHeight 480

TCHAR szWndAppName[] = TEXT("Win_TriangleFox");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 0. ������ ���� �Լ� 
int APIENTRY WinMain(HINSTANCE hInstance,
	 HINSTANCE hPrevInstance,
	 LPSTR    lpCmdLine,
	 int       nCmdShow)
{

	// 1. ������ Ŭ���� ���� 
	
	WNDCLASS wc = { 0 };
	wc.style		  = 0;			   					 // ������ Ŭ������ ��Ÿ�� ����
	wc.lpfnWndProc = WndProc;						     // �޽��� ó�� �Լ��� ������ ���ν��� ������	
	wc.cbClsExtra = 0;                                   // ������ Ŭ���� ������ ���� ���� �ʱⰪ 0  
	wc.cbWndExtra = 0;                                   // ������ ������ ���� ����, �ʱⰪ 0  
	wc.hInstance = hInstance; 				             // �ش� ������ Ŭ������ ����� �������α׷��� �ν��Ͻ�
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				  // �����찡 �ּ�ȭ �ɶ� ������ ������ 	
	wc.hCursor = LoadCursor(0, IDC_IBEAM);			          // �ش� ������ Ŭ������ ������� �������� ���콺 Ŀ�� �ڵ�, IDC_ARROW, IDC_CROSS, IDC_IBEAM, IDC_NO, IDC_WAIT
	wc.hbrBackground  = (HBRUSH)GetStockObject(DKGRAY_BRUSH);  // ������ �۾� ������ ĥ�� ��� �귯�� �ڵ� (���)	
	wc.lpszMenuName = NULL;								      // ����� �޴��� �����Ѵ�. 	
	wc.lpszClassName  = szWndAppName;				          // ����ϰ��� �ϴ� ������ Ŭ���� �̸� 
	
	if (!RegisterClass(&wc))
		return -1;
		
	
	// 2. ������ ���� 
	HWND   hWnd;   //
	hWnd = CreateWindow(
		szWndAppName,
		szWndAppName,
		WS_OVERLAPPED | WS_SYSMENU,
		(ScreenWidth - WindowWidth) / 2,
		(ScreenHeight - WindowHeight) / 2,
		WindowWidth,
		WindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	
	// 3. ������ ȭ�� ��� 
	ShowWindow(hWnd, nCmdShow);

	// 4. ��ü �ʱ�ȭ.
	GDIManager* WinGDIManager = new GDIManager;
	if (WinGDIManager == nullptr) return 0;

	WinGDIManager->Initialize(hWnd);

	Rasterizer* WinRasterizer = new Rasterizer;
	if (WinRasterizer == nullptr) return 0;

	WinRasterizer->Initialize(WinGDIManager);
	WinRasterizer->SetTriangleList();

	TimeCounter* WinTimeCounter = new TimeCounter;
	if (WinTimeCounter == nullptr) return 0;

	if (WinTimeCounter->Initialize() == false) return 0;


	// 5. �޽��� ���� 
	MSG Message;
	
	while (GetMessage(&Message, 0, 0, 0)) //�޽��� �������� �޽��� ����(WM_QUIT�̸� FALSE ��ȯ)
	{
		TranslateMessage(&Message);       //WM_KEYDOWN�̰� Ű�� ���� Ű�� �� WM_CHAR �߻�
		DispatchMessage(&Message);        //�ݹ� ���ν����� ������ �� �ְ� ����ġ ��Ŵ

		WinTimeCounter->Frame();

		WinRasterizer->DrawTriangleList();
		
		TCHAR TitleString[100];
		_stprintf_s(TitleString, 100, TEXT("%s  //  FPS: %.2f / DeltaTime: %.2fms"), szWndAppName, WinTimeCounter->GetFPS(), WinTimeCounter->GetDeltaTime());
		SetWindowText(hWnd, TitleString);
	}

	// 6. ��ü ����.
	if (WinGDIManager != nullptr)
	{
		WinGDIManager->Release(hWnd);
		delete WinGDIManager;
	}
	if (WinRasterizer != nullptr)
	{
		WinRasterizer->Release();
		delete WinRasterizer;
	}
	if (WinTimeCounter != nullptr)
	{
		delete WinTimeCounter;
	}

	return 0;
}

// 0.1. ������ ���ν��� 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);	
}