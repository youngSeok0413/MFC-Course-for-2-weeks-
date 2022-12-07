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
		HDC h_dc = GetDC(hWnd);

		HPEN h_pen = CreatePen(PS_DASH, 5, RGB(10, 0, 100));
		HPEN old_pen = (HPEN)SelectObject(h_dc, h_pen);

		Rectangle(h_dc, 10, 10, 100, 100);

		SelectObject(h_dc, old_pen);
		DeleteObject(h_pen);
		ReleaseDC(hWnd, h_dc);
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

	SelectObject �Լ��� DC�� ����� GDI ������Ʈ�� �ڵ鰪�� ������ �� ����Ѵ�.
	HGDIOBJ SelectObject(HDC hdc, HGDIOBJ hgdiobj) : �ٲ� DC, ��Ʈ��(�޸� DC�� ��츸 ����), �귯��, �� �� ������Ʈ

	���� �����̳� ���¸� ����
	HPEN CreatePen(int fnPenStyle, int nWidth, COLORREF crColor) : ���� ����(PS_STYLE), �� ����(�ȼ� ����), �� �� - ��ȯ : ���ο� ���� �ڵ鷯

	Stock Object ���(�����쿡�� �̸� �����ϴ� �ڿ��(���� �� ��� ������Ʈ(���� �����)))
	HGDIOBJ GetStockObject(int fnObject) : �ʿ��� ������Ʈ ���� - ��ȯ : �ش� ������Ʈ �ڵ鷯
	���ǻ��� : ������Ʈ ���Ÿ� �ϸ� �ȵ�(DeleteObject, CloseHandle ��� ����)(���� ���� �ڿ��� �ƴϱ� ����)

	�귯�� �� ä��� �Ӽ��� �ִ� �귯��
	HBRUSH CreateSolidBrush(COLORREF crColor), HBRUSH CreateHatchBrush(int fnStyle, COLORREF crColor) : HS_STYLE, �� - �ڵ鷯 ��ȯ

	�ӽ� ����(���Ǽ� ����, �Ͻ���)
	COLORREF SetDCBrushColor(HDC hdc, COLORREF crColor) : �⺻������ DC_BRUSH ������Ʈ�� ��� �;���
	COLORREF SetDCPenColor(HDC hdc, COLORREF crColor) : �⺻������ DC_PEN ������Ʈ�� ��� �;���

	������ ��� ������ �ð� �������� �ݺ����� �۾��� �ϰ� ���� ��� 
	UINT_PTR SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT nElapse, void(CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD))
		: Ÿ�̸Ӹ� ����� �������� �ڵ鰪, Ÿ�̸��� ������ȣ(���� ���� Ÿ�̸Ӹ� ����� �� �ֱ� ������ : ���� ��ȣ�� ���� ���), �ݺ��� �ð� ����(ms ����), ȣ��� �Լ��� �ּ�(CALLBACK �Լ�)
		-> void CALLBACK MyTimerProc(HWND hWNd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime){
			}
	BOOL KillTimer(HWND hWnd, UINT_PTR nIDEvent) : Ÿ�̸� �����ϱ�
*/
