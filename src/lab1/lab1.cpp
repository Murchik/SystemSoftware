#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "../../include/utils.hpp"

// Определение функции задающей поведение окна
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR ptrCmdLine,
                   int nCmdShow)
{
    // Регистрация класса окна
    const wchar_t className[] = L"WindowTemplate";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(0,                            // Дополнительные стили окон
                               className,                    // Класс окна
                               L"Dynamic Background Window", // Текст окна
                               WS_OVERLAPPEDWINDOW,          // Стиль окна

                               // Размер и позиция
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

                               NULL,      // Родительское окно
                               NULL,      // Меню
                               hInstance, // Дескриптор экземпляра
                               NULL       // Дополнительные данные приложения
    );
    if (!hwnd)
    {
        MessageBox(NULL, L"Не удалось создать окно!", L"Выход", MB_OK);
        return NULL;
    }

    // Задание режима отображения окна (Прим. полноэкранный, оконный и т.д.)
    ShowWindow(hwnd, nCmdShow);

    // Обработка сообщений из очереди сообщений процесса
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,
                         UINT uMsg,
                         WPARAM wParam,
                         LPARAM lParam)
{
    static wchar_t text[2] = {};
    static COLORREF color = WHITE;
    switch (uMsg)
    {
    case WM_PAINT:
        paint(hwnd, text, color);
        return 0;
    case WM_KEYDOWN:
        keyHandler(hwnd, wParam, text, color);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
