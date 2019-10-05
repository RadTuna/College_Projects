#include <Windows.h>
#include "resource.h"

#define ScreenWidth 1920
#define ScreenHeight 1080
#define WindowWidth 600
#define WindowHeight 600

TCHAR szWndAppName[] = TEXT("MiniZelda");

HINSTANCE gInstance;

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
	wc.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);  // ������ �۾� ������ ĥ�� ��� �귯�� �ڵ� (���)	
	wc.lpszMenuName = NULL;								      // ����� �޴��� �����Ѵ�. 	
	wc.lpszClassName  = szWndAppName;				          // ����ϰ��� �ϴ� ������ Ŭ���� �̸� 
	
	if (!RegisterClass(&wc))
		return -1;
		
	
	// 2. ������ ���� 
	HWND   hWnd;   //
	hWnd = CreateWindow(
		szWndAppName,
		szWndAppName,
		WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME,
		(ScreenWidth - WindowWidth) / 2,
		(ScreenHeight - WindowHeight) / 2,
		WindowWidth,
		WindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	gInstance = hInstance;
	
	// 3. ������ ȭ�� ��� 
	ShowWindow(hWnd, nCmdShow);

	//MessageBox(NULL, TEXT("Hello û�����б�"), TEXT("Hello"), MB_OK);


	// 4. �޽��� ���� 
	MSG Message;
	
	while (GetMessage(&Message, 0, 0, 0)) //�޽��� �������� �޽��� ����(WM_QUIT�̸� FALSE ��ȯ)
	{
		TranslateMessage(&Message);       //WM_KEYDOWN�̰� Ű�� ���� Ű�� �� WM_CHAR �߻�
		DispatchMessage(&Message);        //�ݹ� ���ν����� ������ �� �ְ� ����ġ ��Ŵ
	}

	return 0;
}

// 0.1. ������ ���ν��� 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HDC MemDC;
	PAINTSTRUCT ps;
	HBITMAP MyBitmap;
	HBITMAP OldBitmap;

	constexpr int CollisionDistance = 40;
	constexpr int MovementFactor = 2;

	static int CharPosX = 250;
	static int CharPosY = 250;

	static int Trea1PosX = 100;
	static int Trea1PosY = 100;
	static bool Trea1IsDestroy = false;

	static int Trea2PosX = 400;
	static int Trea2PosY = 400;
	static bool Trea2IsDestroy = false;

	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_UP:
				CharPosY -= MovementFactor;
				break;
			case VK_DOWN:
				CharPosY += MovementFactor;
				break;
			case VK_LEFT:
				CharPosX -= MovementFactor;
				break;
			case VK_RIGHT:
				CharPosX += MovementFactor;
				break;
			}

			if (abs(CharPosX - Trea1PosX) < CollisionDistance && abs(CharPosY - Trea1PosY) < CollisionDistance)
			{
				Trea1IsDestroy = true;
			}
			if (abs(CharPosX - Trea2PosX) < CollisionDistance && abs(CharPosY - Trea2PosY) < CollisionDistance)
			{
				Trea2IsDestroy = true;
			}

			InvalidateRect(hWnd, nullptr, true);
			return 0;
			
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			MemDC = CreateCompatibleDC(hdc);

			// Treasure
			if (Trea1IsDestroy == false || Trea2IsDestroy == false)
			{
				MyBitmap = LoadBitmap(gInstance, MAKEINTRESOURCE(IDB_BITMAP2));
				OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
				if (Trea1IsDestroy == false)
				{
					StretchBlt(hdc, Trea1PosX, Trea1PosY, 24, 24, MemDC, 0, 0, 240, 240, SRCCOPY);
				}
				if (Trea2IsDestroy == false)
				{
					StretchBlt(hdc, Trea2PosX, Trea2PosY, 24, 24, MemDC, 0, 0, 240, 240, SRCCOPY);
				}
				SelectObject(MemDC, OldBitmap);
				DeleteObject(MyBitmap);
			}

			// Character
			MyBitmap = LoadBitmap(gInstance, MAKEINTRESOURCE(IDB_BITMAP1));
			OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
			StretchBlt(hdc, CharPosX, CharPosY, 36, 33, MemDC, 0, 0, 365, 338, SRCCOPY);
			SelectObject(MemDC, OldBitmap);
			DeleteObject(MyBitmap);

			DeleteDC(MemDC);
			EndPaint(hWnd, &ps);
			return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);	
}