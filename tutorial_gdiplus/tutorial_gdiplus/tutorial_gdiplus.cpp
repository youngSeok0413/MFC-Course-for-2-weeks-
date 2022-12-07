// tutorial_gdiplus.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "tutorial_gdiplus.h"

// Direct2D를 사용하기 위한 헤더 파일과 라이브러리 파일을 포함시킨다.
#include <d2d1.h>
#pragma comment(lib, "D2D1.lib")
// Direct2D에서 자주 사용할 네임 스페이스를 생략할 수 있도록 설정한다.
using namespace D2D1;

// Direct2D를 구성하는 각종 객체를 생성하는 객체
ID2D1Factory* gp_factory;
// Direct2D에서 윈도우의 클라이언트 영역에 그림을 그릴 객체
ID2D1HwndRenderTarget* gp_render_target;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_PAINT) {
		ValidateRect(hWnd, NULL);  // WM_PAINT 메시지가 다시 발생하지 않게 만든다.
		// Direct2D의 Render Target을 사용해서 그림 그리기를 시작한다.
		gp_render_target->BeginDraw();
		// BeginDraw 함수를 사용하면 Render Target 전체 영역이 검은색으로 변경되기
		// 때문에 Clear 함수를 사용하여  윈도우 전체 영역을 하늘색으로 채운다.
		gp_render_target->Clear(ColorF(0.0f, 0.8f, 1.0f));

		D2D1_ELLIPSE my_region;
		// 타원의 중심점을 (250, 250)으로 설정한다.
		my_region.point.x = 100;
		my_region.point.y = 100;
		my_region.radiusX = 50.0f;  // 타원의 X축(수평)방향 반지름
		my_region.radiusY = 50.0f;  // 타원의 Y축(수직)방향 반지름

		ID2D1SolidColorBrush* p_yellow_brush = NULL;
		// 원을 그리는데 사용할 Brush 객체를 노란색으로 생성한다.
		gp_render_target->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 0.0f), &p_yellow_brush);
		// 지정한 영역에 노란색 Brush 로 타원을 그린다.
		gp_render_target->FillEllipse(my_region, p_yellow_brush);
		// 사용하던 Brush 객체를 제거한다.
		p_yellow_brush->Release();
		p_yellow_brush = NULL;

		// Render Target을 사용해서 그림 그리기를 중지한다.
		gp_render_target->EndDraw();
		return 0;
	}
	else if (uMsg == WM_CREATE) {
		RECT r;
		GetClientRect(hWnd, &r);  // 클라이언트 영역의 좌표를 얻는다.
		// 지정한 윈도우의 클라이언트 영역에 그림을 그리기 위한 
		// Render Target을 생성한다.
		gp_factory->CreateHwndRenderTarget(RenderTargetProperties(),
			HwndRenderTargetProperties(hWnd, SizeU(r.right, r.bottom)),
			&gp_render_target);

		return 0;
	}
	else if (uMsg == WM_DESTROY) {
		// 사용하던 Render Target이 있다면 제거한다.
		if (gp_render_target != NULL) {
			gp_render_target->Release();
			gp_render_target = NULL;
		}
		// WM_QUIT를 메시지 큐에 넣어서 프로그램을 종료시킴
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 컴포넌트를 사용할 수 있도록 프로그램을 초기화한다.
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// Direct2D를 위한 Factory 객체를 생성한다.
	if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gp_factory)) return 0;

	WNDCLASS wc;
	wchar_t my_class_name[] = L"tipssoft";

	wc.cbClsExtra = NULL;
	wc.cbWndExtra = 0;
	// Direct2D를 사용해서 클라이언트 영역 전체를 그리기 때문에
	// 배경은 별도로 그리지 않아도 될 것 같아서 배경에 사용할 Brush 객체를 사용하지 않는다.
	wc.hbrBackground = (HBRUSH)NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = my_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	HWND h_wnd = CreateWindow(my_class_name, L"www.tipssoft.com - Direct2D",
		WS_OVERLAPPEDWINDOW, 50, 50, 500, 300, NULL, NULL, hInstance, NULL);

	ShowWindow(h_wnd, SW_SHOW);
	UpdateWindow(h_wnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// 사용하던 Factory 객체를 제거한다.
	gp_factory->Release();
	// 컴포넌트 사용을 해제한다.
	CoUninitialize();

	return (int)msg.wParam;
}

/*
* GDI++
	#include <Gdiplus.h>
	#pragma comment(lib, "gdiplus")
	using namespace Gdiplus;
* Direct 2D 사용하기
	COM으로 만들어져 있기 때문에 COM 라이브러리를 사용하기 위해서는 ColniitializeEx(NULL, COINIT_APARATMENTTHREADED) 함수를 호출해주면 된다(한 번만 호출하면 된다.)
	프로그램 종료전에는 반대 역할인 CoUninitialize()

	Factory 객체가 필요하다(Direct2D를 구성하는 각종 객체를 생성하는 객체이다.)

	#include <d2d1.h>
	#pragma comment(lib, "D2D1.lib")
	using namespace D2D1;
*/

