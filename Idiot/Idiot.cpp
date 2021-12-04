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
// The above copyright notice and this permission notice shall be included in all
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
// v1.1.0
//

#include "resource.h"

// Windows Header Files
#include <windows.h>
#include <commctrl.h>

// C RunTime Header Files
#include <math.h>

//constants
#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 225
#define FLASH_TICKS 25
#define SPAWN_NEW_WINDOWS 4

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void    CALLBACK WindowTimerProc(HWND hWnd, UINT message, UINT_PTR nTimerID, DWORD nMillis);

//global variables
HIMAGELIST himlIdiot;
bool createSeparateProcesses = false;

struct IdiotWindow{
	int offsetX = 5;
	int offsetY = 5;
	bool black = false;
	bool firstRun;
	int flashTimer = rand() % FLASH_TICKS;
	HWND hWnd = nullptr;
	const HINSTANCE hInst = GetModuleHandleW(nullptr);

	IdiotWindow(bool firstRun = false);
	INT Draw();
};


IdiotWindow::IdiotWindow(bool firstRun):firstRun(firstRun){
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUPWINDOW;

	RECT size = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	AdjustWindowRect(&size, dwStyle, FALSE);

	hWnd = CreateWindowExW(WS_EX_TOPMOST,
		L"IdiotWindowClass",
		L"You Are An Idiot",
		dwStyle,
		rand() % GetSystemMetrics(SM_CXSCREEN),
		rand() % GetSystemMetrics(SM_CYSCREEN),
		size.right - size.left,
		size.bottom - size.top,
		NULL,
		NULL,
		hInst,
		NULL
	);
	if(!hWnd){
		throw;
	}

	SetPropW(hWnd, L"ClassPointer", this);

	ShowWindow(hWnd, firstRun ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE);//SW_SHOWMAXIMIZED
	UpdateWindow(hWnd);

	SetTimer(hWnd, (UINT_PTR)this, 10, WindowTimerProc);
	PlaySoundW(MAKEINTRESOURCEW(IDR_IDIOT), hInst, SND_LOOP | SND_ASYNC | SND_RESOURCE | SND_NOSTOP);
}

INT IdiotWindow::Draw(){
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	//draw background image
	ImageList_Draw(himlIdiot, black, hdc, 0, 0, ILD_IMAGE);
	EndPaint(hWnd, &ps);
	return 0;
}

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);

	int num = 0;
	LPWSTR* t = CommandLineToArgvW(lpCmdLine, &num);
	if(num && !wcscmp(t[0], L"--dangerous")){
		createSeparateProcesses = true;
	}

	// Specify seed for rand() functions
	LARGE_INTEGER seed;
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);

	// Register window class
	WNDCLASSEXW wcex = {0};
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"IdiotWindowClass";
	RegisterClassExW(&wcex);

	// Load images
	HANDLE imageWhite = LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_IDIOT_WHITE), IMAGE_ICON, 0, 0, 0);
	HANDLE imageBlack = LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_IDIOT_BLACK), IMAGE_ICON, 0, 0, 0);

	// Create image list
	himlIdiot = ImageList_Create(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 2, 0);
	ImageList_AddIcon(himlIdiot, (HICON)imageWhite);
	ImageList_AddIcon(himlIdiot, (HICON)imageBlack);

	// Create first window
	new IdiotWindow(true);

	MSG msg;
	// Main message loop:
	while(GetMessageW(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return (int)msg.wParam;
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
	IdiotWindow* WindowPtr = (IdiotWindow*)GetPropW(hWnd, L"ClassPointer");
	switch (message){
		case WM_PAINT:
			return WindowPtr->Draw();
		case WM_CLOSE:
			if(WindowPtr->firstRun){
				DestroyWindow(WindowPtr->hWnd);
				//ShowWindow(WindowPtr->hWnd, SW_SHOWNORMAL);
				WindowPtr->firstRun = false;
				//make user think window was closed
				Sleep(2000);
			}
			// On close open new 4 windows
			for(int i = 0; i < SPAWN_NEW_WINDOWS; i++){
				if(createSeparateProcesses){
					WCHAR currentFile[MAX_PATH];
					GetModuleFileNameW(nullptr, currentFile, MAX_PATH);
					ShellExecuteW(hWnd, L"open", currentFile, L"--dangerous", nullptr, SW_SHOWNORMAL);
					Sleep(500);
				}
				else{
					IdiotWindow* wnd = new IdiotWindow();
				}
			}
			return 0;
		case WM_DESTROY:
			if(createSeparateProcesses){
				PostQuitMessage(0);
			}
			break;
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}



void CALLBACK WindowTimerProc(HWND hWnd, UINT message, UINT_PTR nTimerID, DWORD nMillis){

	IdiotWindow* WindowPtr = (IdiotWindow*)nTimerID;
	if(!WindowPtr->firstRun || createSeparateProcesses){
		//get current window position
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);

		int wWidth = windowRect.right - windowRect.left;
		int wHeight = windowRect.bottom - windowRect.top;

		//if window reaches edge of screen generate new offset
		if(windowRect.left > GetSystemMetrics(SM_CXSCREEN) - wWidth){
			WindowPtr->offsetX = newNegative();
		}
		if(windowRect.left < 0){
			WindowPtr->offsetX = newPositive();
		}
		if(windowRect.top > GetSystemMetrics(SM_CYSCREEN) - wHeight){
			WindowPtr->offsetY = newNegative();
		}
		if(windowRect.top < 0){
			WindowPtr->offsetY = newPositive();
		}

		//add offset to current window position
		windowRect.left += WindowPtr->offsetX;
		windowRect.top += WindowPtr->offsetY;

		MoveWindow(hWnd, windowRect.left, windowRect.top, wWidth, wHeight, FALSE);
	}

	WindowPtr->flashTimer++;
	if(WindowPtr->flashTimer > FLASH_TICKS){
		WindowPtr->flashTimer = 0;
		WindowPtr->black = !WindowPtr->black;
		InvalidateRect(WindowPtr->hWnd, nullptr, TRUE);
	}
}