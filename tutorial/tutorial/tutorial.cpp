// tutorial.cpp : Defines the entry point for the application.
//
#include "pch.h"
#include "framework.h"
#include "tutorial.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		HDC hdc = GetDC(hWnd);
		HDC h_mem_dc = CreateCompatibleDC(hdc);

		HBITMAP h_bitmap = CreateCompatibleBitmap(hdc, 400, 300);

		SelectObject(hdc, h_bitmap);

		Rectangle(h_mem_dc, 10, 10, 100, 100);

		DeleteObject(h_bitmap);
		DeleteDC(h_mem_dc);
		ReleaseDC(hWnd, hdc);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	WNDCLASS wc;

	wchar_t my_class_name[] = L"Tutorial";
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = my_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW|CS_VREDRAW;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(my_class_name, my_class_name,
		WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0,0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

/*
* Bitmap : ARGB�� �����ϰ�, �޸𸮸� ���� ���� BGRA unsigned int/ unsigned char(32/8)
* HBITMAP CreateBitMap(int nWidth, int nHeight, UINT nPlanes, UINT nBitCount, CONSTvoid* lpBits)
* HBITMAP CreateCompatibleBitmap(HDC hdc, int nWidth, int nHeight)
* BITMAP : �ش� ������ ������ ���� �����.
* 
* typedef struct tagBITMAP{
*		LONG bmType; //0
*		LONG bmWidth; //��Ʈ���� ��
		LONG bmHeight; // ��Ʈ���� ����
		LONG bmWidthBytes; //���� �������� �� �ٿ� ���Ǵ� ����Ʈ ���� ���
		WORD bmPlanes; //��Ʈ�ʿ��� ������ ǥ���ϱ� ���� �÷� ��(��κ��� 1, 16���� : 4)
		WORD bmBitsPixel; //�� ���� ǥ���ϴ� �� ���Ǵ� ��Ʈ ��
		LPVOID bmBits; //��Ʈ���� �̹��� ������ ����Ǿ� �ִ� �޸��� ���� �ּҸ� ���
	}

	GDI Object : Bitmap, Pen, Brush ... -> �� ��� ���� ���� DC(������Ʈ���� �ڵ鰪�� �����س��� ��Ʈ��Ʈ) : HDC : DC ���� �ٷ�� ���� ��ü�� �ڵ� ��

	DC�� ����� ��Ʈ�� �ڵ� �� ���
	HGDIOBJ GetCurrentObject(HDC hdc, UINT uObjectType) : HBRUSH(OBJ_BRUSH), HPEN(OBJ_PEN), HFONT(OBJ_FONT), HPALETTE(OBJ_PAL).. -> HBITMAP���� ����ȯ ����/ ������ ���� ����
	�ڵ��� �̿��Ͽ� ������Ʈ ���� ��� : lpvObject ����� �����Ѵ�.
	int GetObject(HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject) : ������Ʈ �ڵ�, �޾ƾ��ϴ� ������Ʈ ���� ũ��, ����Ǵ� ��ġ(�� ����)
	��Ʈ���� �����ؼ� ���Ǵ� ���, ������ ��Ʈ���� �ü���� �����ϰ� �� : �� ���� ������ �ڵ鷯�� �̿�

	��Ʈ ������ ��°�� �����ؿ��� �Լ�
	LONG GetBitmapBits(HBITMAP hbmp, LONG cbBuffer, LPVOID lpvBits) : �ڵ鷯, ������ ũ��, ����Ǵ� ��ġ/ ��ȯ : ������ ��Ʈ������ ũ��(��Ʈ ������ ����(�� ����))
	���ο� ��Ʈ ������ �ش� ��Ʈ �������� �����ϴ� �Լ�
	LONG SetBitmapBits(HBITMAP hbmp, DWORD cb, const LPVOID lpvBits) : ����� SetDlBits�� �ٲ�, ���� �ٲ� �ڵ鰪, ������ ����Ʈ ��, ���ο� ��Ʈ ������ ����� �޸� �ּ� : ��ȯ�� : ������ ������ ��Ʈ������ ũ��
	��ǻ���� ���� ������ �ʿ��� ���
	int GetSystem(int nIndex) : SM_MESSAGE : �ʿ��� ������ ����� ������ ����, �ܼ��� ���°����� ���� �� �ִ�
	
	�׸� �׸��� ���ؼ��� GDI �������� �ʿ��ϰ� �̵��� �ٷ�� ���ؼ��� DC �� �ʿ��ϴ�.
	DC ����
	HDC GetDC(HWND hWnd) : ���� �����쿡 DE�� ������ �´�. �� �� ������� ������ �� DC�� BITMAP ��ü�� ȭ�鿡 ��� ���� ������ �����ϱ� ������ SelectObject�� ������ �Ұ����ϴ�, NULL�� �Ű������� ������ ��ü ȭ��
	DC �ݳ� : ��� ������ ������ �ֱ� ������ DC �Լ��� �ݺ��ؼ� ����ϸ� ���̻� DC �Լ��� ����� �� ����(������ �ݳ�)
	int ReleaseDC(HWND hWnd, HDC hdc) : ��� �����쿡�� � HDC�� �ݳ��ϴ� ��
	������ ������ �ڵ�κ��� DC�� �޴� ���, ��Ʈ���� ������ �� �� �ۿ� ����. �ٸ� PEN�̳� BRUSH�� ������ ����Ǳ� ������ �� ��쿡�� �ǹ̰� �ִ�

	BOOL GetCursorPos(LPPOINT lpPoint) : POINT ��ȯ ��ġ/���콺�� ���� ��ġ�� ��ȯ�Ѵ�.
	CPoint : �� ���� ����� �����ϴ� ����(Ŭ������� �����ϸ� ����)

	WM_PAINT : ��ȿȭ ������ �ٽ� �׸���(�������� ������ �����϶�� �޼���)
	typedef struct PAINTSTRUCT{
		HDC hdc; //BeginPaint �Լ��� ���� DC�� �ڵ� ��
		BOOL fErase; //�������� �⺻ ����� �ٽ� �׸��� ����
		RECT rcPaint; //�ٽ� �׷����� ���� ����
		BOOL fRestore; //�ü�� ����
		BOOL fIncUpdate; //�ü�� ����
		BYTE rgbdReserved[32]; //�ü�� ����
	}
	WM_PAINT ����(�ݵ�� �� �� �����, �޼����� ó���Ǿ��� �� Ȯ���ϴ� ����� GetD���� ����) �׸��� ȭ�� ����� �켱 ������ ����, ���δ� �����ϴ� ��쵵  �ִ�(�÷��� �޼������ ��, ���̺� �����)
	BeginPaint(hWnd, &ps)
	��ȯ
	EndPaint(hWnd, &ps);

	WM_CREATE : �����츦 �����϶�� �޼���(���� ���� �߻��ϴ� ���: �̴ϼ� ��ɰ� ����), CreateWindow �Լ� ȣ���ϸ� �ش� �۾��� ��û�� ���α׷����� ���޵Ǵ� �޼���/�����̸� 0�� ��ȯ, 
		���� ������ -1 ��ȯ, ��ȯ�ؾ��� �����찡 ��μ� ��������� ���� �Ϸ�ȴ�.
	lParam���� �������� ���� ������ ������ ���� 
	typedef struct tagCREATESTRUCTA {
		LPVOID    lpCreateParams;
		HINSTANCE hInstance;
		HMENU     hMenu;
		HWND      hwndParent;
		int       cy;
		int       cx;
		int       y;
		int       x;
		LONG      style;
		LPCSTR    lpszName;
		LPCSTR    lpszClass;
		DWORD     dwExStyle;
	}

	DC�� DC ���� �׸��� ������ �� �ִ� 
	BOOL BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop)
		: �纻 HDC, �纻���� ����� �̹����� �����ϴ� x, �纻���� ����� �̹����� �����ϴ� y, �纻 ����, �纻 ����, ���� HDC, �������� ���� ���� ��ġ, ����ɼ� 

	HDC CreateCompatibleDC(HDC hdc) : ������ �Ǵ� hdc - DeleteDC(hdc)�� �����ؾ��Ѵ�

	WindowRect ���ü��� �Լ��� ���� ���ݾ� �޶���
	GetWindowRect() : ����� ȭ���� �������� �ϴ� ���� ��ǥ��
	GetClientRect : ���� �����츦 ���� ����� ��ǥ��(Ŭ���̾�Ʈ ������ ��������)

	WM_PAINT �޼��� ���� �߻� : BOOL ValidateRect(HWND hWnd, const RECT *lpRect) : �ش� ������ ��ȿȭ�� �� ������ WM_PAINT �޼���  �߻� <-> BOOL InValidateRect(HWND hWnd, const RECT *lpRect)
	NULL�� ��ü ȭ��, ValidateRect�� ����ϸ� WM_MESSAGE�� ��� �߻��ϴ� ������ ��������.
	
	�޼��� �ڽ�
	int MessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) : �ڵ鷯, ����, ����, �޼��� �ڽ� Ÿ��(OR �������� �߰� ���� MB_�ɼ�) : ��ȯ�ϴ� ���� ����ڰ� �޼��� �ڽ��� ������ ��

	WM_CLOSE : ������ �� �߻��ϴ� ��� : �޼��� �ڽ� ���� �߰��ϴ� �͵� ������ �ʴ�
	WM_DESTROY : �̹� �����츦 �ı���(���⼭�� �� �� �� ����)
	WM_MOVING : �����찡 �����϶� �۵�, lParam�� ���� �������� ���� ��ġ�� ����Ǿ� ���� : WM_MOVING���� ��
*/
