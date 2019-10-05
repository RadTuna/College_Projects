#include <Windows.h>
#include "resource.h"

#define ScreenWidth 1920
#define ScreenHeight 1080
#define WindowWidth 600
#define WindowHeight 600

TCHAR szWndAppName[] = TEXT("MiniZelda");

HINSTANCE gInstance;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 0. 윈도우 메인 함수 
int APIENTRY WinMain(HINSTANCE hInstance,
	 HINSTANCE hPrevInstance,
	 LPSTR    lpCmdLine,
	 int       nCmdShow)
{

	// 1. 윈도우 클래스 정의 
	
	WNDCLASS wc = { 0 };
	wc.style		  = 0;			   					 // 윈도우 클래스의 스타일 종류
	wc.lpfnWndProc = WndProc;						     // 메시지 처리 함수인 윈도우 프로시저 포인터	
	wc.cbClsExtra = 0;                                   // 윈도우 클래스 데이터 예약 영역 초기값 0  
	wc.cbWndExtra = 0;                                   // 윈도우 데이터 예약 영역, 초기값 0  
	wc.hInstance = hInstance; 				             // 해당 윈도우 클래스를 등록한 응용프로그램의 인스턴스
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				  // 윈도우가 최소화 될때 보여줄 아이콘 	
	wc.hCursor = LoadCursor(0, IDC_IBEAM);			          // 해당 윈도우 클래스로 만들어진 윈도우의 마우스 커서 핸들, IDC_ARROW, IDC_CROSS, IDC_IBEAM, IDC_NO, IDC_WAIT
	wc.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);  // 윈도우 작업 영역을 칠할 배경 브러쉬 핸들 (흰색)	
	wc.lpszMenuName = NULL;								      // 사용할 메뉴를 지정한다. 	
	wc.lpszClassName  = szWndAppName;				          // 등록하고자 하는 윈도우 클래스 이름 
	
	if (!RegisterClass(&wc))
		return -1;
		
	
	// 2. 윈도우 생성 
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
	
	// 3. 윈도우 화면 출력 
	ShowWindow(hWnd, nCmdShow);

	//MessageBox(NULL, TEXT("Hello 청강대학교"), TEXT("Hello"), MB_OK);


	// 4. 메시지 루프 
	MSG Message;
	
	while (GetMessage(&Message, 0, 0, 0)) //메시지 루프에서 메시지 꺼냄(WM_QUIT이면 FALSE 반환)
	{
		TranslateMessage(&Message);       //WM_KEYDOWN이고 키가 문자 키일 때 WM_CHAR 발생
		DispatchMessage(&Message);        //콜백 프로시저가 수행할 수 있게 디스패치 시킴
	}

	return 0;
}

// 0.1. 윈도우 프로시저 
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