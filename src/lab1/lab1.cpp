#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "../../include/utils.hpp"

// Определение функции задающей поведение окна
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Буфер для символов отображаемых на экране
wchar_t text[2] = {};
// Цвет фона окна и фона символов
COLORREF color = WHITE;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR ptrCmdLine, int nCmdShow) {
    // Обработка параметра командной строки и получение цвета
    LPWSTR cliArgs = GetCommandLine();
    getColorFromStr(cliArgs, color);
    HBRUSH hBrushBgColor = CreateSolidBrush(color);

    // Регистрация класса окна
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = hBrushBgColor;
    wc.lpszClassName = L"DynamicBackgroundWindow";
    if (!RegisterClass(&wc)) {
        ErrorExit(TEXT("RegisterClass"));
        return 0;
    }

    // Создание окна
    HWND hwnd = CreateWindow(wc.lpszClassName,  // Класс окна
                             L"Dynamic Background Window",  // Текст окна
                             WS_OVERLAPPEDWINDOW,  // Стиль окна
                             100, 100,             // Позиция
                             640, 480,             // Размер
                             NULL,       // Родительское окно
                             NULL,       // Меню
                             hInstance,  // Дескриптор экземпляра
                             NULL);  // Дополнительные данные приложения
    if (!hwnd) {
        ErrorExit(TEXT("Create main window"));
        return 0;
    }

    // Задание режима отображения окна (Прим. полноэкранный, оконный и т.д.)
    ShowWindow(hwnd, nCmdShow);

    // Обработка сообщений из очереди сообщений процесса
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        // Преобразует сообщения виртуального ключа в символьные сообщения.
        // Символьные сообщения отправляются в очередь сообщений вызывающего
        // потока, чтобы быть прочитанными при следующем вызове потоком функции
        // GetMessage или PeekMessage.
        TranslateMessage(&msg);

        // Каждый раз, когда программа вызывает функцию DispatchMessage, она
        // косвенно заставляет Windows вызывать функцию WindowProc один раз для
        // каждого сообщения.
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_MYCASE:
            text[0] = ' ';
            color = (COLORREF)wParam;
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        case WM_PAINT:
            drawText(hwnd, text, color);
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
