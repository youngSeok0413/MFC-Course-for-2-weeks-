// exercise_1.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "exercise_1.h"

#define X_COUNT 19
#define Y_COUNT 19

#define X_START 50
#define Y_START 50

#define INTERVAL 26
#define HALF_INTERVAL INTERVAL/2

#define XPOS(x) (X_START + (x)*INTERVAL)
#define YPOS(y) (Y_START + (y)*INTERVAL)

unsigned char g_dol[Y_COUNT][X_COUNT]; //0,1,2 : 빈 공간/ 검은 돌/ 흰 돌
unsigned char g_step;

//마우스 클릭으로 바둑판에 돌 놓기
void OnLButtonDown(HWND ah_wnd, int a_x, int a_y) {
	//왼 쪽 버튼을 클릭했을 때 사용할 코드
	//영역 안으로 들어감
	if (a_x > XPOS(0) - HALF_INTERVAL && a_y > YPOS(0) - HALF_INTERVAL
		&& a_x < XPOS(X_COUNT-1) + HALF_INTERVAL && a_y < YPOS(Y_COUNT-1) + HALF_INTERVAL) {
		
	}
}

//페인트 함수
void OnPaint(HWND ah_wnd) {
	PAINTSTRUCT ps;
	HDC h_dc = BeginPaint(ah_wnd, &ps);

	//줄 그리는 함수
	for (int i = 0; i < X_COUNT; i++) {
		MoveToEx(h_dc, XPOS(i), YPOS(0), NULL);
		LineTo(h_dc, XPOS(i), YPOS(Y_COUNT - 1));
	}

	for (int i = 0; i < Y_COUNT; i++) {
		MoveToEx(h_dc, XPOS(0), YPOS(i), NULL);
		LineTo(h_dc, XPOS(X_COUNT - 1), YPOS(i));
	}

	for (int y = 0; y < Y_COUNT; y++) {
		for (int x = 0; x < X_COUNT; x++) {
			if (g_dol[y][x] > 0 ) {
				if (g_dol[y][x] == 1) {
					//검은 돌
					SelectObject(h_dc, GetStockObject(BLACK_BRUSH));
				}
				else {
					//흰 돌
					SelectObject(h_dc, GetStockObject(WHITE_BRUSH));
				}
				Ellipse(h_dc, XPOS(x)-HALF_INTERVAL, YPOS(y) - HALF_INTERVAL, 
					XPOS(x) + HALF_INTERVAL, YPOS(y) + HALF_INTERVAL);
			}
		}
	}

	EndPaint(ah_wnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (uMsg == WM_PAINT) {
		OnPaint(hWnd);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASS wc;

	wchar_t my_class_name[] = L"오목";
	HBRUSH crBackgorund = CreateSolidBrush(RGB(244, 176, 77));
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = crBackgorund;
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = my_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(my_class_name, my_class_name,
		WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(crBackgorund);
	return msg.wParam;
}
