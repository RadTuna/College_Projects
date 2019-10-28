#include <Windows.h>

#define ScreenWidth 1920
#define ScreenHeight 1080
#define WindowWidth 640
#define WindowHeight 480

TCHAR szWndAppName[] = TEXT("WndTest");

HINSTANCE gHInstance;

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

	gHInstance = hInstance;
	
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
	enum { QUIZ_MAX = 3, ID_R1 = 100, ID_R2 = 101, ID_R3 = 102, ID_R4 = 103, ID_BT = 104, ID_ST = 105 };

	static TCHAR quesList[3][64] = { TEXT("Quiz 1\n32비트 운영체제에서 int형의 크기는 몇인가?"), TEXT("Quiz 2\n렌더링 파이프라인에서 가장 뒤에 있는 것은 무엇인가?"), TEXT("Quiz 3\nVector4연산을 한번의 인스트럭션으로 할 수 있는 방법은?") };
	static TCHAR ansList[3][4][32] = { { TEXT("8bit"), TEXT("16bit"), TEXT("32bit"), TEXT("64bit") }, { TEXT("정점쉐이더"), TEXT("래스터라이저"), TEXT("입력 병합기"), TEXT("픽셀쉐이더") }, { TEXT("SIMD"), TEXT("SHA-256"), TEXT("NTSC"), TEXT("COM") } };
	static int corList[3] = { 2, 3, 0 };
	static TCHAR endList[4][32] = { TEXT("다시 도전하세요."), TEXT("노력해주세요."), TEXT("훌륭합니다."), TEXT("당신은 퀴즈의 신입니다.") };
	static HWND hStatic;
	static HWND hRadio1, hRadio2, hRadio3, hRadio4;
	static HWND hBtn;
	static HWND hGroup;
	static HDC hdc;
	static PAINTSTRUCT ps;
	static int currentQuiz = 0;
	static int correctQuiz = 0;
	static int selectAnswer = 0;

	switch (message)
	{
	case WM_CREATE:
		hStatic = CreateWindow(TEXT("static"), TEXT("Quiz"), WS_CHILD | WS_VISIBLE,
			10, 10, 410, 40, hWnd, (HMENU)ID_ST, gHInstance, nullptr);
		hGroup = CreateWindow(TEXT("button"), TEXT("Answer"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			10, 70, 410, 110, hWnd, (HMENU)0, gHInstance, nullptr);
		hRadio1 = CreateWindow(TEXT("button"), TEXT("A1"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
			20, 100, 150, 30, hWnd, (HMENU)ID_R1, gHInstance, nullptr);
		hRadio2 = CreateWindow(TEXT("button"), TEXT("A2"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			180, 100, 150, 30, hWnd, (HMENU)ID_R2, gHInstance, nullptr);
		hRadio3 = CreateWindow(TEXT("button"), TEXT("A3"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			20, 140, 150, 30, hWnd, (HMENU)ID_R3, gHInstance, nullptr);
		hRadio4 = CreateWindow(TEXT("button"), TEXT("A4"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			180, 140, 150, 30, hWnd, (HMENU)ID_R4, gHInstance, nullptr);
		hBtn = CreateWindow(TEXT("button"), TEXT("OK"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			340, 140, 70, 30, hWnd, (HMENU)ID_BT, gHInstance, nullptr);
		CheckRadioButton(hWnd, ID_R1, ID_R4, ID_R1);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			switch (LOWORD(wParam))
			{
			case ID_R1:
				selectAnswer = 0;
				break;
			case ID_R2:
				selectAnswer = 1;
				break;
			case ID_R3:
				selectAnswer = 2;
				break;
			case ID_R4:
				selectAnswer = 3;
				break;
			case ID_BT:
				if (corList[currentQuiz] == selectAnswer)
				{
					MessageBox(hWnd, TEXT("훌륭합니다."), TEXT("Log"), MB_OK);
					correctQuiz++;
				}
				else
				{
					MessageBox(hWnd, TEXT("틀렸습니다."), TEXT("Log"), MB_OK);
					
				}
				currentQuiz++;
				InvalidateRect(hWnd, nullptr, true);
				break;
			}
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (currentQuiz < QUIZ_MAX)
		{
			SetWindowText(hStatic, quesList[currentQuiz]);
			SetWindowText(hRadio1, ansList[currentQuiz][0]);
			SetWindowText(hRadio2, ansList[currentQuiz][1]);
			SetWindowText(hRadio3, ansList[currentQuiz][2]);
			SetWindowText(hRadio4, ansList[currentQuiz][3]);
		}
		else
		{
			SetWindowText(hStatic, endList[correctQuiz]);
			DestroyWindow(hRadio1);
			DestroyWindow(hRadio2);
			DestroyWindow(hRadio3);
			DestroyWindow(hRadio4);
			DestroyWindow(hGroup);
			DestroyWindow(hBtn);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);	
}