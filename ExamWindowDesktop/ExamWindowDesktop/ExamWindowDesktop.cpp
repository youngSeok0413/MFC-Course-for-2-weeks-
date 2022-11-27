#include "pch.h"
#include "framework.h"
#include "ExamWindowDesktop.h"

//������� �޼��� ó���Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//window Ŭ������ ��� : ������ �ü���� �ߺ��� ���̱� ���ؼ� �ߺ������� ���α׷��� �ƴ� ������� �ٿ��� �ߺ��� �ذ��Ͽ���. - �����ɾ� �ߺ��� ���ϴ� ���
	WNDCLASS wc;

	wchar_t my_class_name[] = L"tipssoft"; 

	wc.cbClsExtra = NULL;//������ ������ Ŭ������ ����ϴ� ��������� ������ �� �ִ� �޸��� ũ�⸦ �����մϴ�(�� �����Ǿ�� �ϴ� ����, �� 40����Ʈ�� ������ ����, SetClassWord, SetClassLong/GetClassWord, GetClassLong - ���� �� �۰ſ��� ����(�ٸ� ����� ������ : ���� : ���ȼ� ���))
	wc.cbWndExtra = NULL;//�� ������ Ŭ������ ����ϴ� �����츦 ���� ���� �޸𸮸� �߰�����, �� �޸𸮴� �������� ������, ���������� ���� SetWindowLong - GetWindowLong, less than 40bytes
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; //������ ����� �׸� �� ����� Brush Object�� ���� �ڵ鰪 ����
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //������ Ŭ���̾�Ʈ ������ ���콺�� ��ġ���� �� ���콺 Ŀ���� ���� �ڵ鰪�� ����
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //�ΰ� �̹����� ����� �� �ִ� �������� �ڵ鰪�� ����
	wc.hInstance = hInstance; //hInstance : ���� ���α׷��� �����ϱ� ���� ������
	wc.lpfnWndProc = WndProc; // �ش� �������� ó���Լ�(�ݹ� �Լ�)
	wc.lpszClassName = my_class_name;// Ŭ������ �̸� : �ߺ��� ���ϴ� ���� ����(������ ������ �ü���� ����� �����ڵ� ���ڿ�)
	wc.lpszMenuName = NULL; //�����찡 ����� �޴��̸� ����
	wc.style = CS_HREDRAW | CS_VREDRAW; //�������� �׸��� Ư���� ���õ� ������ �����ϰų� �� ���� ����� ȯ���� �߰��ϰų� �����ϴ� ���� ����

	RegisterClass(&wc); //�ش� ������ Ŭ������ �������� Ŭ������ �̿��Ͽ� ������ �ý��ۿ� ���(������ �˾Ƶδ� ��)

	//������ ����(�̹� ��ϵ� �����츦 ���μ��� �� �Ǿ� ������ ��)
	HWND hWnd = CreateWindow(my_class_name, L"www.tipssoft.com",
		WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//���α׷��� ���޵� �޼����� �����ϰ� �����ϴ� �۾�
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}