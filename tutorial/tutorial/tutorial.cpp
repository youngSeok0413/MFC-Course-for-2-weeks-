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
* Bitmap : ARGB로 저장하고, 메모리를 읽을 때는 BGRA unsigned int/ unsigned char(32/8)
* HBITMAP CreateBitMap(int nWidth, int nHeight, UINT nPlanes, UINT nBitCount, CONSTvoid* lpBits)
* HBITMAP CreateCompatibleBitmap(HDC hdc, int nWidth, int nHeight)
* BITMAP : 해당 파일은 다음과 같이 생겼다.
* 
* typedef struct tagBITMAP{
*		LONG bmType; //0
*		LONG bmWidth; //비트맵의 폭
		LONG bmHeight; // 비트맵의 높이
		LONG bmWidthBytes; //가로 방향으로 한 줄에 사용되는 바이트 수를 명시
		WORD bmPlanes; //비트맵에서 색상을 표현하기 위한 플랜 수(대부분이 1, 16색상 : 4)
		WORD bmBitsPixel; //한 점을 표현하는 데 사용되는 비트 수
		LPVOID bmBits; //비트맵의 이미지 패턴이 저장되어 있는 메모리의 시작 주소를 명시
	}

	GDI Object : Bitmap, Pen, Brush ... -> 을 모아 놓은 것이 DC(오브젝트들의 핸들값을 저장해놓은 스트럭트) : HDC : DC 값을 다루기 위한 객체의 핸들 값

	DC에 연결된 비트맵 핸들 값 얻기
	HGDIOBJ GetCurrentObject(HDC hdc, UINT uObjectType) : HBRUSH(OBJ_BRUSH), HPEN(OBJ_PEN), HFONT(OBJ_FONT), HPALETTE(OBJ_PAL).. -> HBITMAP에서 형변환 가능/ 포인터 같은 느낌
	핸들을 이용하여 오브젝트 정보 얻기 : lpvObject 여기로 복사한다.
	int GetObject(HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject) : 오브젝트 핸들, 받아야하는 오브젝트 변수 크기, 복사되는 위치(값 복사)
	비트맵을 생성해서 사용되는 경우, 생성된 비트맵을 운영체제가 관리하게 됨 : 쓸 일이 있으면 핸들러를 이용

	비트 패턴을 통째로 복사해오는 함수
	LONG GetBitmapBits(HBITMAP hbmp, LONG cbBuffer, LPVOID lpvBits) : 핸들러, 복사할 크기, 복사되는 위치/ 반환 : 복사한 비트패턴의 크기(비트 패턴을 복사(값 복사))
	새로운 비트 패턴을 해당 비트 패턴으로 설정하는 함수
	LONG SetBitmapBits(HBITMAP hbmp, DWORD cb, const LPVOID lpvBits) : 현재는 SetDlBits로 바꿈, 패턴 바꿀 핸들값, 설정할 바이트 수, 새로운 비트 패턴이 저장된 메모리 주소 : 반환값 : 실제로 설정한 비트패턴의 크기
	컴퓨터의 설정 정보가 필요한 경우
	int GetSystem(int nIndex) : SM_MESSAGE : 필요한 정보가 상수로 정해져 있음, 단순한 상태값만을 얻을 수 있다
	
	그림 그리기 위해서는 GDI 정보들이 필요하고 이들을 다루기 위해서는 DC 가 필요하다.
	DC 구성
	HDC GetDC(HWND hWnd) : 현재 위도우에 DE를 가지고 온다. 단 이 방식으로 가지고 온 DC는 BITMAP 객체는 화면에 출력 중인 패턴을 유지하기 때문에 SelectObject로 변경이 불가능하다, NULL을 매개변수로 받으면 전체 화면
	DC 반납 : 사용 개수에 제한이 있기 때문에 DC 함수를 반복해서 사용하면 더이상 DC 함수를 사용할 수 없다(권한의 반납)
	int ReleaseDC(HWND hWnd, HDC hdc) : 어느 윈도우에서 어떤 HDC를 반납하는 가
	동일한 윈도우 핸들로부터 DC를 받는 경우, 비트맵은 공유를 할 수 밖에 없다. 다만 PEN이나 BRUSH는 별개로 저장되기 때문에 이 경우에는 의미가 있다

	BOOL GetCursorPos(LPPOINT lpPoint) : POINT 반환 위치/마우스의 현재 위치를 반환한다.
	CPoint : 더 많은 기능을 제공하는 변수(클래스라고 생각하면 편해)

	WM_PAINT : 무효화 영역과 다시 그리기(지워졌던 영역을 복구하라는 메세지)
	typedef struct PAINTSTRUCT{
		HDC hdc; //BeginPaint 함수로 얻은 DC의 핸들 값
		BOOL fErase; //윈도우의 기본 배경을 다시 그릴지 여부
		RECT rcPaint; //다시 그려야할 영역 정보
		BOOL fRestore; //운영체제 영역
		BOOL fIncUpdate; //운영체제 영역
		BYTE rgbdReserved[32]; //운영체제 영역
	}
	WM_PAINT 전용(반드시 이 거 써야함, 메세지가 처리되었는 지 확인하는 기능은 GetD에는 없다) 그리고 화면 출력은 우선 순위가 낮고, 때로는 무시하는 경우도  있다(플래그 메세지라고 함, 테이블에 저장됨)
	BeginPaint(hWnd, &ps)
	반환
	EndPaint(hWnd, &ps);

	WM_CREATE : 윈도우를 생성하라는 메세지(제일 먼저 발생하는 명령: 이니셜 명령과 같음), CreateWindow 함수 호출하면 해당 작업을 요청한 프로그램으로 전달되는 메세지/정상이면 0을 변환, 
		문제 있으면 -1 반환, 반환해야지 윈도우가 비로소 만들어지는 것이 완료된다.
	lParam에는 윈도우의 생성 정보를 가지고 있음 
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

	DC와 DC 간에 그림을 복사할 수 있는 
	BOOL BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop)
		: 사본 HDC, 사본으로 복사된 이미지가 시작하는 x, 사본으로 복사된 이미지가 시작하는 y, 사본 넓이, 사본 높이, 원본 HDC, 원본에서 복사 시작 위치, 복사옵션 

	HDC CreateCompatibleDC(HDC hdc) : 기준이 되는 hdc - DeleteDC(hdc)로 제거해야한다

	WindowRect 포시션이 함수에 따라서 조금씩 달라짐
	GetWindowRect() : 모니터 화면을 기준으로 하는 절대 좌표계
	GetClientRect : 개별 윈도우를 위한 상대적 좌표계(클라이언트 영역을 기준으로)

	WM_PAINT 메세지 강제 발생 : BOOL ValidateRect(HWND hWnd, const RECT *lpRect) : 해당 영역을 무효화한 후 강제로 WM_PAINT 메세지  발생 <-> BOOL InValidateRect(HWND hWnd, const RECT *lpRect)
	NULL은 전체 화면, ValidateRect를 사용하면 WM_MESSAGE가 계속 발생하는 문제는 없어진다.
	
	메세지 박스
	int MessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) : 핸들러, 내용, 제목, 메세지 박스 타입(OR 조건으로 추가 가능 MB_옵션) : 반환하는 것은 사용자가 메세지 박스를 조작한 값

	WM_CLOSE : 종료할 때 발생하는 명령 : 메세지 박스 정도 추가하는 것도 나쁘지 않다
	WM_DESTROY : 이미 윈도우를 파괴함(여기서는 뭘 할 수 없음)
	WM_MOVING : 윈도우가 움직일때 작동, lParam에 현재 윈도우의 절대 위치가 저장되어 있음 : WM_MOVING까지 함
*/
