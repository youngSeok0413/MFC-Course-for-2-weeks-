// exercise_imagefile.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "exercise_imagefile.h"

#include <d2d1.h>
#include <wincodec.h>
#pragma comment(lib, "D2D1.lib")

using namespace D2D1;

ID2D1Factory* gp_factory;
ID2D1HwndRenderTarget* gp_render_target;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_CREATE) {
	}
	else if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gp_factory);

	WNDCLASS wc;
	wchar_t my_class_name[] = L"Image file in/out";

	wc.hbrBackground = (HBRUSH)WHITE_BRUSH;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = my_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW|CS_VREDRAW;
	RegisterClass(&wc);

	HWND hWnd = CreateWindow(my_class_name, my_class_name, 
		WS_OVERLAPPEDWINDOW, 50, 50, 672, 504, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	gp_factory->Release();

	CoUninitialize();

	return (int)msg.wParam;

}

/*
* WIC, 이미지 파일 입출력을 담당할 여려 함수들이 포함되어 있음
*/
