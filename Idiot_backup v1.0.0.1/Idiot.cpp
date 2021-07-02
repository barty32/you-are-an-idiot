// Idiot.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Idiot.h"

#define MAX_LOADSTRING 100
#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 225

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//entries:
//	HWND	hwndWindow;
//	INT		xPos;
//	INT		yPos;
//	INT		xOffset;
//	INT		yOffset;
//	BOOL	bBlack;
//	UINT	nTimerID;
//typedef struct _tagWINDOWDATA{
//	HWND	hwndWindow;		//handle to associated window
//	INT		xPos;			//current x position of the window
//	INT		yPos;			//current y position of the window
//	INT		xOffset;		//current x offfset
//	INT		yOffset;		//current y offset
//	BOOL	bBlack;			//current background color
//	UINT	nTimerID;		//id of timer
//}WINDOWDATA, *LPWINDOWDATA;
//

//WINDOWDATA* arrData = NULL;
//INT arrDataSize = 0;
int offsetsX[4096];
int offsetsY[4096];
BOOL blacks[4096];

HIMAGELIST himlIdiot;
//BOOL bBlack = FALSE;

UINT nTimers = 0;

HANDLE xH;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	InitCommonControls();

	srand((UINT)time(0));

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_IDIOT, szWindowClass, MAX_LOADSTRING);

	if(!MyRegisterClass(hInstance)){
		return FALSE;
	}

	for(int i = 0; i < 4096; i++){
		offsetsX[i] = 5;
		offsetsY[i] = 5;
		blacks[i] = 0;
	}

	HANDLE imageWhite = LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_IDIOT_WHITE), IMAGE_ICON, 0, 0, 0);
	HANDLE imageBlack = LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_IDIOT_BLACK), IMAGE_ICON, 0, 0, 0);

	himlIdiot = ImageList_Create(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 2, 0);

	ImageList_AddIcon(himlIdiot, (HICON)imageWhite);
	ImageList_AddIcon(himlIdiot, (HICON)imageBlack);

	hInst = hInstance; // Store instance handle in our global variable

	// Perform application initialization:
	if(!InitInstance(hInstance, nCmdShow)){
		return FALSE;
	}

	HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_IDIOT));

	MSG msg;

	// Main message loop:
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance){
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_IDIOT));
	wcex.hIconSm        = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(IDI_IDIOT));
	wcex.hCursor        = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = szWindowClass;
	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){

   DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUPWINDOW;

   RECT size = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
   AdjustWindowRect(&size, dwStyle, FALSE);

   //if(arrDataSize <= 0){
	//   arrData = (LPWINDOWDATA)malloc(sizeof(WINDOWDATA));
	//   if(arrData == NULL){
	//	   return 0;
	//   }
	//   arrDataSize = 1;
	//   arrData[0] = WINDOWDATA{0};
   //}
   //else{
	//   arrData = (LPWINDOWDATA)realloc(arrData, (arrDataSize + 1) * sizeof(WINDOWDATA));
	//   if(arrData == NULL){
	//	   return 0;
	//   }
	//   arrDataSize++;
	//   arrData[arrDataSize - 1] = WINDOWDATA{0};
   //}
   //
   //arrData[arrDataSize - 1].xPos = rand() % GetSystemMetrics(SM_CXSCREEN);
   //arrData[arrDataSize - 1].yPos = rand() % GetSystemMetrics(SM_CYSCREEN);

   HWND hWnd = CreateWindowExW(
		0,//WS_EX_TOPMOST,
		szWindowClass,
		szTitle,
		dwStyle,
		rand() % GetSystemMetrics(SM_CXSCREEN),
		rand() % GetSystemMetrics(SM_CYSCREEN),
		size.right - size.left,
		size.bottom - size.top, 
		NULL, 
		NULL, 
		hInstance, 
		NULL
	);

   if (!hWnd){
	  return FALSE;
   }

   //arrData[arrDataSize - 1].hwndWindow = hWnd;
   //arrData[arrDataSize - 1].nTimerID = arrDataSize;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}




int newNegative(){
	return ((int)ceil(0.0 - (6.0 * ((double)(rand()) / 32768.0)))) * 5 - 10;
}
int newPositive(){
	return ((int)ceil((7.0 * ((double)(rand()) / 32768.0)))) * 5 - 10;
}

//DWORD WINAPI ThreadProc(LPVOID lpParameter){
	//InitInstance(hInst, SW_SHOWNOACTIVATE);
	//WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
	//WaitForSingleObject(*(HANDLE*)lpParameter, INFINITE);
	//int errora = GetLastError();
	//return 0;
//}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	//static BOOL bBlack = false;
	//static int xOff = 5;
	//static int yOff = 5;
	static int temp = 0;

	switch (message){
		case WM_CREATE:
		{
			
			nTimers++;
			SetTimer(hWnd, nTimers, 500, NULL);
			nTimers++;
			SetTimer(hWnd, nTimers, 1, NULL);
			PlaySoundW(MAKEINTRESOURCEW(IDR_IDIOT), GetModuleHandle(NULL), SND_LOOP | SND_ASYNC | SND_RESOURCE | SND_NOSTOP);
			break;
		}
		case WM_TIMER:
		{
			if(wParam % 2){
				//bBlack = !bBlack;
				blacks[wParam] = !blacks[wParam];
				//SendMessage(hWnd, WM_PAINT, blacks[wParam], 0);
				temp = blacks[wParam];
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if(!(wParam % 2)){

				RECT windowRect;
				GetWindowRect(hWnd, &windowRect);
				static int wWidth = windowRect.right - windowRect.left;
				static int wHeight = windowRect.bottom - windowRect.top;


				if(windowRect.left > GetSystemMetrics(SM_CXSCREEN) - wWidth){
					//xOff = newNegative();
					offsetsX[wParam] = newNegative();
				}
				if(windowRect.left < 0){
					//xOff = newPositive();
					offsetsX[wParam] = newPositive();
				}
				if(windowRect.top > GetSystemMetrics(SM_CYSCREEN) - wHeight){
					//yOff = newNegative();
					offsetsY[wParam] = newNegative();
				}
				if(windowRect.top < 0){
					//yOff = newPositive();
					offsetsY[wParam] = newPositive();
				}
				windowRect.left += offsetsX[wParam];// xOff;
				windowRect.top += offsetsY[wParam];// yOff;
				MoveWindow(hWnd, windowRect.left, windowRect.top, wWidth, wHeight, FALSE);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			
			//static BOOL TEMP = wParam;
			if(!ImageList_Draw(himlIdiot, temp/*bBlack*/, hdc, 0, 0, ILD_IMAGE)){
				//int error = GetLastError();
				//int a = 0;
			}
		
			//Rectangle(hdc, 0, 0, 60, 60);
			//RECT screenRectangle;
			//GetClientRect(hWnd, &screenRectangle);
			//FillRect(hdc, &screenRectangle, CreateSolidBrush(RGB(0, 255, 0)));
			//screenRectangle.left += 3;
			//screenRectangle.top += 3;
			//screenRectangle.bottom -= 3;
			//screenRectangle.right -= 3;
			//FillRect(hdc, &screenRectangle, CreateSolidBrush(RGB(255, 0, 0)));
		
		
			//SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
			//SetTextColor(hdc, RGB(0, 0, 255));
			//SetBkMode(hdc, TRANSPARENT);
			
			//TextOutW(hdc, 100, 150, L"You Are An Idiot!", 18);
			//DrawTextW(hdc, L"You Are An Idiot!", 18, &screenRectangle, DT_CENTER | DT_VCENTER);
		
		
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
		{
			InitInstance(hInst, SW_SHOWNOACTIVATE);
			InitInstance(hInst, SW_SHOWNOACTIVATE);
			InitInstance(hInst, SW_SHOWNOACTIVATE);
			InitInstance(hInst, SW_SHOWNOACTIVATE);
			// 
			//HANDLE hNew = NULL;
			//hNew = CreateThread(NULL, 0, ThreadProc, &hNew, 0, NULL);
			//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitInstance, NULL, 0, NULL);
			//Sleep(1000);
			//ThreadProc(NULL);

			//STARTUPINFOW si;
			//PROCESS_INFORMATION pi;

			//ZeroMemory(&si, sizeof(si));
			//si.cb = sizeof(STARTUPINFOW);
			//ZeroMemory(&pi, sizeof(pi));
			
			//WCHAR szExePath[MAX_PATH];
			//GetModuleFileNameW(NULL, szExePath, MAX_PATH);

			//CreateProcessW(szExePath, NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
			//int error = GetLastError();
			//CloseHandle(pi.hProcess);
			//CloseHandle(pi.hThread);

			//InitInstance(hInst, 10);
			//InitInstance(hInst, 10);
			//InitInstance(hInst, 10);
			//PostQuitMessage(0);
		}
		break;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}


//void CALLBACK Blink(HWND hWnd, UINT message, UINT_PTR nTimerID, DWORD nMillis){
//	arrData[arrDataSize - 1].bBlack = !arrData[arrDataSize - 1].bBlack;
//	InvalidateRect(hWnd, NULL, TRUE);
//}


//void CALLBACK MoveTimer(HWND hWnd, UINT message, UINT_PTR nTimerID, DWORD nMillis){
//	int xOff = 5;
//	int yOff = 5;
//
//	RECT windowRect;
//	GetWindowRect(hWnd, &windowRect);
//	static int wWidth = windowRect.right - windowRect.left;
//	static int wHeight = windowRect.bottom - windowRect.top;
//
//	
//	if(windowRect.left > GetSystemMetrics(SM_CXSCREEN) - wWidth){
//		xOff = newNegative();
//	}
//	if(windowRect.left < 0){
//		xOff = newPositive();
//	}
//	if(windowRect.top > GetSystemMetrics(SM_CYSCREEN) - wHeight){
//		yOff = newNegative();
//	}
//	if(windowRect.top < 0){
//		yOff = newPositive();
//	}
//	windowRect.left += xOff;
//	windowRect.top += yOff;
//	MoveWindow(hWnd, windowRect.left, windowRect.top, wWidth, wHeight, FALSE);
//
//}