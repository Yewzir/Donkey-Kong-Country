/*
 * * * * * * * * * * * * * * * * * * * * * * * * *
 *        Copyright (c) 2024 Hicham Omari        *
 *                                               *
 * https://github.com/Yewzir/Donkey-Kong-Country *
 * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int iCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	RECT rcWindow, rcClient;
	MSG msg;
	int iTicks;
	static int iTicksTrigger;

	ZeroMemory(&wc, sizeof(wc));

	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = L"DKC";

	if (!RegisterClass(&wc) || !(hWnd = CreateWindow(L"DKC", L"Donkey Kong Country", WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX, 0, 0, 256, 224, nullptr, nullptr, hInstance, nullptr)))
		return 0;

	GetWindowRect(hWnd, &rcWindow);
	GetClientRect(hWnd, &rcClient);

	int iWidth = rcWindow.right * 2 - rcClient.right,
		iHeight = rcWindow.bottom * 2 - rcClient.bottom;

	SetWindowPos(hWnd, nullptr, (GetSystemMetrics(SM_CXSCREEN) - iWidth) / 2, (GetSystemMetrics(SM_CYSCREEN) - iHeight) / 2, iWidth, iHeight, SWP_NOZORDER);

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (iTicksTrigger < (iTicks = int(GetTickCount64()))) {
				InvalidateRect(hWnd, nullptr, false);
				iTicksTrigger = iTicks + 1000 / 15;
			}
		}
	}
	return int(msg.wParam);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	HDC hDevice, hMemory;
	PAINTSTRUCT ps;
	HBITMAP hBitmap, hPrevBitmap;

	GetClientRect(hWnd, &rc);

	switch (msg) {
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		hDevice = BeginPaint(hWnd, &ps);
		hMemory = CreateCompatibleDC(hDevice);

		hBitmap = CreateCompatibleBitmap(hDevice, rc.right, rc.bottom);
		hPrevBitmap = HBITMAP(SelectObject(hMemory, hBitmap));

		BitBlt(hDevice, rc.left, rc.top, rc.right, rc.bottom, hMemory, 0, 0, SRCCOPY);

		SelectObject(hDevice, hPrevBitmap);
		DeleteObject(hBitmap);

		DeleteDC(hMemory);
		EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
