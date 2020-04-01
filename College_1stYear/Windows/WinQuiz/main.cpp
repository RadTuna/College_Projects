#include <Windows.h>

#define ScreenWidth 1920
#define ScreenHeight 1080
#define WindowWidth 640
#define WindowHeight 480

TCHAR szWndAppName[] = TEXT("WndTest");

HINSTANCE gHInstance;

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

	gHInstance = hInstance;
	
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
	enum { QUIZ_MAX = 3, ID_R1 = 100, ID_R2 = 101, ID_R3 = 102, ID_R4 = 103, ID_BT = 104, ID_ST = 105 };

	static TCHAR quesList[3][64] = { TEXT("Quiz 1\n32��Ʈ �ü������ int���� ũ��� ���ΰ�?"), TEXT("Quiz 2\n������ ���������ο��� ���� �ڿ� �ִ� ���� �����ΰ�?"), TEXT("Quiz 3\nVector4������ �ѹ��� �ν�Ʈ�������� �� �� �ִ� �����?") };
	static TCHAR ansList[3][4][32] = { { TEXT("8bit"), TEXT("16bit"), TEXT("32bit"), TEXT("64bit") }, { TEXT("�������̴�"), TEXT("�����Ͷ�����"), TEXT("�Է� ���ձ�"), TEXT("�ȼ����̴�") }, { TEXT("SIMD"), TEXT("SHA-256"), TEXT("NTSC"), TEXT("COM") } };
	static int corList[3] = { 2, 3, 0 };
	static TCHAR endList[4][32] = { TEXT("�ٽ� �����ϼ���."), TEXT("������ּ���."), TEXT("�Ǹ��մϴ�."), TEXT("����� ������ ���Դϴ�.") };
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
					MessageBox(hWnd, TEXT("�Ǹ��մϴ�."), TEXT("Log"), MB_OK);
					correctQuiz++;
				}
				else
				{
					MessageBox(hWnd, TEXT("Ʋ�Ƚ��ϴ�."), TEXT("Log"), MB_OK);
					
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