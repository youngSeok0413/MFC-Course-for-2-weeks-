// Step5.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Step5.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY) PostQuitMessage(0);
    else if (uMsg == WM_MOUSEMOVE) {
        HDC hdc = GetDC(hWnd);

        if (wParam == MK_LBUTTON) {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            Rectangle(hdc, x - 15, y - 15, x + 15, y + 15);
        }
        else if (wParam == MK_RBUTTON) {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            Ellipse(hdc, x - 15, y - 15, x + 15, y + 15);
        }

        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x + 10, y + 10);

        ReleaseDC(hWnd, hdc); // 그림이 새로워질때마다 MoveTo, LineTo 모두 초기화된다. 그래서 이어지는 그림을 그리고 싶다면 이전상태를 기억해야 한다
    }
    else if (uMsg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps); //WM_PAINT 메세지를 초기화 시켜주는 역할 도 한다
        EndPaint(hWnd, &ps);
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc;

    wchar_t my_class_name[] = L"tipssoft";
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = my_class_name;
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    HWND hWnd = CreateWindow(my_class_name, L"www.tipssoft.com",
        WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


