// MIT License
// 
// Copyright(c) 2021 barty12
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
// Idiot.cpp : A simple Windows joke
// 
// v1.0.1
//

#include "resource.h"

// Windows Header Files
#include <windows.h>
#include <commctrl.h>

// C RunTime Header Files
#include <math.h>
#include <time.h>

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 225

// Forward declarations of functions included in this code module:
BOOL             CreateNewWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


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

	// Specify seed for rand() functions
	srand((UINT)time(0));

	// Register window class
	WNDCLASSEXW wcex = {0};
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_IDIOT));
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = L"IdiotWindowClass";
	RegisterClassExW(&wcex);

	// Initialize memory
	for(int i = 0; i < 4096; i++){
		offsetsX[i] = 5;
		offsetsY[i] = 5;
		blacks[i] = 0;
	}

	// Load images
	HANDLE imageWhite = LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_IDIOT_WHITE), IMAGE_ICON, 0, 0, 0);
	HANDLE imageBlack = LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_IDIOT_BLACK), IMAGE_ICON, 0, 0, 0);

	// Create image list
	himlIdiot = ImageList_Create(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 2, 0);
	ImageList_AddIcon(himlIdiot, (HICON)imageWhite);
	ImageList_AddIcon(himlIdiot, (HICON)imageBlack);

	// Create first window
	if(!CreateNewWindow(hInstance, nCmdShow)){
		return FALSE;
	}

	MSG msg;
	// Main message loop:
	while(GetMessageW(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return (int)msg.wParam;
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

   if(!hWnd){
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Create new negative offset
INT newNegative(){
	return ((int)ceil(0.0 - (6.0 * ((double)(rand()) / 32768.0)))) * 5 - 10;
}

// Create new positive offset
INT newPositive(){
	return ((int)ceil((7.0 * ((double)(rand()) / 32768.0)))) * 5 - 10;
}


// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	static BOOL bInternalBlackState = 0;
	switch (message){
		case WM_CREATE:
		{
			nTimers++;
			//flash background timer, odd numbers
			SetTimer(hWnd, nTimers, 500, NULL);
			nTimers++;
			//move window timer, even numbers
			SetTimer(hWnd, nTimers, 1, NULL);
			PlaySoundW(MAKEINTRESOURCEW(IDR_IDIOT), GetModuleHandleW(NULL), SND_LOOP | SND_ASYNC | SND_RESOURCE | SND_NOSTOP);
			break;
		}
		case WM_TIMER:
		{
			// Odd numbers
			if(wParam % 2){
				blacks[wParam] = !blacks[wParam];
				bInternalBlackState = blacks[wParam];
				InvalidateRect(hWnd, NULL, TRUE);
			}
			// Even numbers
			else if(!(wParam % 2)){

				//get current window position
				RECT windowRect;
				GetWindowRect(hWnd, &windowRect);
				static int wWidth = windowRect.right - windowRect.left;
				static int wHeight = windowRect.bottom - windowRect.top;

				//if window reaches edge of screen generate new offset
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

				//add offset to current window position
				windowRect.left += offsetsX[wParam];
				windowRect.top += offsetsY[wParam];

				//move window
				MoveWindow(hWnd, windowRect.left, windowRect.top, wWidth, wHeight, FALSE);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			//draw background image
			ImageList_Draw(himlIdiot, bInternalBlackState, hdc, 0, 0, ILD_IMAGE);
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

			//don't quit the process
			//PostQuitMessage(0);
		}
		break;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}