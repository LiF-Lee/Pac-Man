#include <Windows.h>

#include "GameEngine/GameEngine.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Windows Pac-Man");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	WndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	WndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = nullptr;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);

	const HWND hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
	                         100, 100, 586, 698,
	                         NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, nullptr, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return static_cast<int>(Message.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memDC;
	PAINTSTRUCT ps;
	HBITMAP memBitmap, oldBitmap;
	RECT rect;
	
	static GameEngine* gameEngine;
	
	switch (iMessage) {
	case WM_CREATE:
		gameEngine = new GameEngine;
		SetTimer(hWnd, 1, 200, nullptr);
		break;
	case WM_TIMER:
		gameEngine->Update();
		InvalidateRect(hWnd, nullptr, FALSE);
		break;
	case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		memDC = CreateCompatibleDC(hdc);
		memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		oldBitmap = static_cast<HBITMAP>(SelectObject(memDC, memBitmap));
        gameEngine->Paint(memDC);
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
		SelectObject(hdc, oldBitmap);
		DeleteObject(memBitmap);
		DeleteDC(memDC);
        EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			gameEngine->ChangePlayerMovementState(STATE_UP);
			break;
		case VK_DOWN:
			gameEngine->ChangePlayerMovementState(STATE_DOWN);
			break;
		case VK_LEFT:
			gameEngine->ChangePlayerMovementState(STATE_LEFT);
			break;
		case VK_RIGHT:
			gameEngine->ChangePlayerMovementState(STATE_RIGHT);
			break;
		}
		break;
	case WM_DESTROY:
		delete gameEngine;
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}