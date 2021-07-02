#pragma once

#include "resource.h"

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


void CALLBACK Blink(HWND hWnd, UINT message, UINT_PTR nTimerID, DWORD nMillis);
void CALLBACK MoveTimer(HWND hWnd, UINT message, UINT_PTR nTimerID, DWORD nMillis);

//DWORD WINAPI ThreadProc(LPVOID lpParameter);