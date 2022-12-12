// OmokD2D.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "OmokD2D.h"

#include <d2d1.h>
#pragma comment(lib, "D2D1.lib")

using namespace D2D1;

#define X_COUNT 19
#define Y_COUNT 19

#define X_START 50
#define Y_START 50

#define INTERVAL 26
#define HALF_INTERVAL INTERVAL/2

#define XPOS(x) (float)(X_START + (x)*INTERVAL)
#define YPOS(y) (float)(Y_START + (y)*INTERVAL)

unsigned char g_dol[Y_COUNT][X_COUNT];
unsigned char g_step;

ID2D1Factory* gp_factory;
ID2D1HwndRenderTarget* gp_render_target;

void OnPaint(HWND ah_wnd) {

	gp_render_target->BeginDraw();

	gp_render_target->Clear(ColorF(0.95686f, 0.69f, 0.3019f));

	ID2D1SolidColorBrush* p_line_brush, * p_black_brush, * p_white_brush;

	gp_render_target->CreateSolidColorBrush(ColorF(0.095686f, 0.069f, 0.03019f), &p_line_brush);
	gp_render_target->CreateSolidColorBrush(ColorF(0.0f, 0.0f, 0.0f), &p_black_brush);
	gp_render_target->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 1.0f), &p_white_brush);

	D2D1_POINT_2F start_pos, end_pos;
	for (int x = 0; x < X_COUNT; x++) {
		start_pos.x = XPOS(x);
		start_pos.y = YPOS(0);
		end_pos.x = XPOS(x);
		end_pos.y = YPOS(Y_COUNT-1);

		gp_render_target->DrawLine(start_pos, end_pos, p_line_brush, (float)1.0f);
	}

	for (int y = 0; y < X_COUNT; y++) {
		start_pos.x = XPOS(0);
		start_pos.y = YPOS(y);
		end_pos.x = XPOS(X_COUNT-1);
		end_pos.y = YPOS(y);

		gp_render_target->DrawLine(start_pos, end_pos, p_line_brush, (float)1.0f);
	}

	D2D1_ELLIPSE dol_region;
	dol_region.radiusX = (float)HALF_INTERVAL;
	dol_region.radiusY = (float)HALF_INTERVAL;

	for (int y = 0; y < Y_COUNT; y++) {
		for (int x = 0; x < Y_COUNT; x++) {
			if (g_dol[y][x] > 0) {
				dol_region.point.x = XPOS(x);
				dol_region.point.y = YPOS(y);

				if (g_dol[y][x] == 1) gp_render_target->FillEllipse(dol_region, p_black_brush);
				else gp_render_target->FillEllipse(dol_region, p_white_brush);
			}
		}
	}

	p_line_brush->Release();
	p_line_brush = NULL;
	p_black_brush->Release();
	p_black_brush = NULL;
	p_white_brush->Release();
	p_white_brush = NULL;

	gp_render_target->EndDraw();
}

void OnLButtonDown(HWND ah_wnd, int a_x, int a_y) {
	if (a_x > (XPOS(0) - HALF_INTERVAL) && a_y > (YPOS(0) - HALF_INTERVAL)
		&& a_x < (XPOS(X_COUNT-1) + HALF_INTERVAL) && a_y < (YPOS(Y_COUNT-1) + HALF_INTERVAL)) {
		
		int x = (a_x - X_START + HALF_INTERVAL)/INTERVAL;
		int y = (a_y - Y_START + HALF_INTERVAL) / INTERVAL;
		if (g_dol[y][x] == 0) {
			g_dol[y][x] = g_step + 1;
			g_step = !g_step;

			InvalidateRect(ah_wnd, NULL, TRUE);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_CREATE) {
		RECT r;
		GetClientRect(hWnd, &r);

		gp_factory->CreateHwndRenderTarget(RenderTargetProperties(),
			HwndRenderTargetProperties(hWnd, SizeU(r.right, r.bottom)),
			&gp_render_target);

		return 0;
	}
	else if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (uMsg == WM_PAINT) {
		ValidateRect(hWnd, NULL);
		OnPaint(hWnd);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		OnLButtonDown(hWnd, x, y);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gp_factory)) return 0;
	
	WNDCLASS wc;
	wchar_t my_class_name[] = L"Omok uisng Direct2D";

	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.lpszClassName = my_class_name;
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)NULL;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW||CS_VREDRAW;
	RegisterClass(&wc);

	HWND h_Wnd = CreateWindow(my_class_name, my_class_name, WS_OVERLAPPEDWINDOW,
		50, 50, 600, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(h_Wnd, nCmdShow);
	UpdateWindow(h_Wnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	gp_factory->Release();
	CoUninitialize();
	return (int)msg.wParam;
}
