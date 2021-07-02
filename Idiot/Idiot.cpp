// Idiot.cpp : Defines the entry point for the application.
//

#include "resource.h"

//#include <SDKDDKVer.h>
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <commctrl.h>
#include <math.h>
#include <time.h>

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 225

// Forward declarations of functions included in this code module:
ATOM                RegisterWndClass(HINSTANCE hInstance);
BOOL                CreateNewWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


// Global Variables:

int offsetsX[4096];
int offsetsY[4096];
BOOL blacks[4096];

HIMAGELIST himlIdiot;

UINT nTimers = 0;


INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	InitCommonControls();

	srand((UINT)time(0));

	if(!RegisterWndClass(hInstance)){
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

	// Load first window
	if(!CreateNewWindow(hInstance, nCmdShow)){
		return FALSE;
	}

	MSG msg;

	// Main message loop:
	while (GetMessageW(&msg, NULL, 0, 0)){
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}




ATOM RegisterWndClass(HINSTANCE hInstance){
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
	wcex.lpszClassName  = L"IdiotWindowClass";
	return RegisterClassExW(&wcex);
}


BOOL CreateNewWindow(HINSTANCE hInstance, int nCmdShow){

   DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUPWINDOW;

   RECT size = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
   AdjustWindowRect(&size, dwStyle, FALSE);

   HWND hWnd = CreateWindowExW(
		0,
		L"IdiotWindowClass",
		L"You Are In Idiot",
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

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


INT newNegative(){
	return ((int)ceil(0.0 - (6.0 * ((double)(rand()) / 32768.0)))) * 5 - 10;
}

INT newPositive(){
	return ((int)ceil((7.0 * ((double)(rand()) / 32768.0)))) * 5 - 10;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

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
				blacks[wParam] = !blacks[wParam];
				temp = blacks[wParam];
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if(!(wParam % 2)){

				RECT windowRect;
				GetWindowRect(hWnd, &windowRect);
				static int wWidth = windowRect.right - windowRect.left;
				static int wHeight = windowRect.bottom - windowRect.top;


				if(windowRect.left > GetSystemMetrics(SM_CXSCREEN) - wWidth){
					offsetsX[wParam] = newNegative();
				}
				if(windowRect.left < 0){
					offsetsX[wParam] = newPositive();
				}
				if(windowRect.top > GetSystemMetrics(SM_CYSCREEN) - wHeight){
					offsetsY[wParam] = newNegative();
				}
				if(windowRect.top < 0){
					offsetsY[wParam] = newPositive();
				}
				windowRect.left += offsetsX[wParam];
				windowRect.top += offsetsY[wParam];
				MoveWindow(hWnd, windowRect.left, windowRect.top, wWidth, wHeight, FALSE);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			ImageList_Draw(himlIdiot, temp, hdc, 0, 0, ILD_IMAGE);
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
		{
			//on close open new 4 windows
			CreateNewWindow(GetModuleHandle(NULL), SW_SHOWNOACTIVATE);
			CreateNewWindow(GetModuleHandle(NULL), SW_SHOWNOACTIVATE);
			CreateNewWindow(GetModuleHandle(NULL), SW_SHOWNOACTIVATE);
			CreateNewWindow(GetModuleHandle(NULL), SW_SHOWNOACTIVATE);

			//PostQuitMessage(0);
		}
		break;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}