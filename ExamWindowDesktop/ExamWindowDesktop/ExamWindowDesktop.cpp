#include "pch.h"
#include "framework.h"
#include "ExamWindowDesktop.h"

//사용자의 메세지 처리함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//window 클래스의 등록 : 윈도우 운영체제는 중복을 줄이기 위해서 중복단위를 프로그램이 아닌 윈도우로 줄여서 중복을 해결하였다. - 실행명령어 중복을 피하는 방법
	WNDCLASS wc;

	wchar_t my_class_name[] = L"tipssoft"; 

	wc.cbClsExtra = NULL;//동일한 윈도우 클래스를 사용하는 위도우들이 공유할 수 있는 메모리의 크기를 설정합니다(꼭 공유되어야 하는 정보, 단 40바이트를 넘지는 못함, SetClassWord, SetClassLong/GetClassWord, GetClassLong - 저장 및 글거오기 가능(다만 사용은 제한적 : 사유 : 보안성 취약))
	wc.cbWndExtra = NULL;//이 윈도우 클래스를 사용하는 윈도우를 위한 개별 메모리를 추가해줌, 이 메모리는 공유되지 않으며, 개멸적으로 사용됨 SetWindowLong - GetWindowLong, less than 40bytes
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; //윈도우 배경을 그릴 때 사용할 Brush Object에 대한 핸들값 저장
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //윈도우 클라이언트 영역에 마우스가 위치했을 때 마우스 커서에 대한 핸들값을 저장
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //로고 이미지로 사용할 수 있는 아이콘의 핸들값을 저장
	wc.hInstance = hInstance; //hInstance : 응용 프로그램을 구별하기 위한 고유값
	wc.lpfnWndProc = WndProc; // 해당 윈도우의 처리함수(콜백 함수)
	wc.lpszClassName = my_class_name;// 클래스의 이름 : 중복을 피하는 것이 좋다(윈도우 단위로 운영체제가 기억함 유니코드 문자열)
	wc.lpszMenuName = NULL; //윈도우가 사용할 메뉴이름 저장
	wc.style = CS_HREDRAW | CS_VREDRAW; //윈도우의 그리기 특성과 관련된 정보를 설정하거나 몇 가지 사용자 환경을 추가하거나 제한하는 값을 저장

	RegisterClass(&wc); //해당 윈도우 클래스를 레지스터 클래스를 이용하여 윈도우 시스템에 등록(종류를 알아두는 것)

	//윈도우 생성(이미 등록된 윈도우를 프로세스 상에 실어 나르는 것)
	HWND hWnd = CreateWindow(my_class_name, L"www.tipssoft.com",
		WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//프로그램에 전달된 메세지를 번역하고 실항하는 작업
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}