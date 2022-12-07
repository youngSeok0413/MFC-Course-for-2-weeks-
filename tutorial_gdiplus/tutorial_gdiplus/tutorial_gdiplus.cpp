// tutorial_gdiplus.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "tutorial_gdiplus.h"

// Direct2D�� ����ϱ� ���� ��� ���ϰ� ���̺귯�� ������ ���Խ�Ų��.
#include <d2d1.h>
#pragma comment(lib, "D2D1.lib")
// Direct2D���� ���� ����� ���� �����̽��� ������ �� �ֵ��� �����Ѵ�.
using namespace D2D1;

// Direct2D�� �����ϴ� ���� ��ü�� �����ϴ� ��ü
ID2D1Factory* gp_factory;
// Direct2D���� �������� Ŭ���̾�Ʈ ������ �׸��� �׸� ��ü
ID2D1HwndRenderTarget* gp_render_target;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_PAINT) {
		ValidateRect(hWnd, NULL);  // WM_PAINT �޽����� �ٽ� �߻����� �ʰ� �����.
		// Direct2D�� Render Target�� ����ؼ� �׸� �׸��⸦ �����Ѵ�.
		gp_render_target->BeginDraw();
		// BeginDraw �Լ��� ����ϸ� Render Target ��ü ������ ���������� ����Ǳ�
		// ������ Clear �Լ��� ����Ͽ�  ������ ��ü ������ �ϴû����� ä���.
		gp_render_target->Clear(ColorF(0.0f, 0.8f, 1.0f));

		D2D1_ELLIPSE my_region;
		// Ÿ���� �߽����� (250, 250)���� �����Ѵ�.
		my_region.point.x = 100;
		my_region.point.y = 100;
		my_region.radiusX = 50.0f;  // Ÿ���� X��(����)���� ������
		my_region.radiusY = 50.0f;  // Ÿ���� Y��(����)���� ������

		ID2D1SolidColorBrush* p_yellow_brush = NULL;
		// ���� �׸��µ� ����� Brush ��ü�� ��������� �����Ѵ�.
		gp_render_target->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 0.0f), &p_yellow_brush);
		// ������ ������ ����� Brush �� Ÿ���� �׸���.
		gp_render_target->FillEllipse(my_region, p_yellow_brush);
		// ����ϴ� Brush ��ü�� �����Ѵ�.
		p_yellow_brush->Release();
		p_yellow_brush = NULL;

		// Render Target�� ����ؼ� �׸� �׸��⸦ �����Ѵ�.
		gp_render_target->EndDraw();
		return 0;
	}
	else if (uMsg == WM_CREATE) {
		RECT r;
		GetClientRect(hWnd, &r);  // Ŭ���̾�Ʈ ������ ��ǥ�� ��´�.
		// ������ �������� Ŭ���̾�Ʈ ������ �׸��� �׸��� ���� 
		// Render Target�� �����Ѵ�.
		gp_factory->CreateHwndRenderTarget(RenderTargetProperties(),
			HwndRenderTargetProperties(hWnd, SizeU(r.right, r.bottom)),
			&gp_render_target);

		return 0;
	}
	else if (uMsg == WM_DESTROY) {
		// ����ϴ� Render Target�� �ִٸ� �����Ѵ�.
		if (gp_render_target != NULL) {
			gp_render_target->Release();
			gp_render_target = NULL;
		}
		// WM_QUIT�� �޽��� ť�� �־ ���α׷��� �����Ŵ
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ������Ʈ�� ����� �� �ֵ��� ���α׷��� �ʱ�ȭ�Ѵ�.
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// Direct2D�� ���� Factory ��ü�� �����Ѵ�.
	if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gp_factory)) return 0;

	WNDCLASS wc;
	wchar_t my_class_name[] = L"tipssoft";

	wc.cbClsExtra = NULL;
	wc.cbWndExtra = 0;
	// Direct2D�� ����ؼ� Ŭ���̾�Ʈ ���� ��ü�� �׸��� ������
	// ����� ������ �׸��� �ʾƵ� �� �� ���Ƽ� ��濡 ����� Brush ��ü�� ������� �ʴ´�.
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

	// ����ϴ� Factory ��ü�� �����Ѵ�.
	gp_factory->Release();
	// ������Ʈ ����� �����Ѵ�.
	CoUninitialize();

	return (int)msg.wParam;
}

/*
* GDI++
	#include <Gdiplus.h>
	#pragma comment(lib, "gdiplus")
	using namespace Gdiplus;
* Direct 2D ����ϱ�
	COM���� ������� �ֱ� ������ COM ���̺귯���� ����ϱ� ���ؼ��� ColniitializeEx(NULL, COINIT_APARATMENTTHREADED) �Լ��� ȣ�����ָ� �ȴ�(�� ���� ȣ���ϸ� �ȴ�.)
	���α׷� ���������� �ݴ� ������ CoUninitialize()

	Factory ��ü�� �ʿ��ϴ�(Direct2D�� �����ϴ� ���� ��ü�� �����ϴ� ��ü�̴�.)

	#include <d2d1.h>
	#pragma comment(lib, "D2D1.lib")
	using namespace D2D1;
*/

