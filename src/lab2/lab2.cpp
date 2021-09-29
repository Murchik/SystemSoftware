#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "../../include/utils.hpp"

// Определение функции задающей поведение окна
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR ptrCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"FormWindow";
    if (!RegisterClass(&wc)) {
        ErrorExit(TEXT("RegisterClass"));
        return 0;
    }

    // Создание окна
    HWND hwnd = CreateWindow(wc.lpszClassName,     // Класс окна
                             L"Lab2",              // Текст окна
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
    // Буферы для строк из полей ввода
    static WCHAR createText[512] = {};
    static WCHAR updateText[512] = {};
    switch (uMsg) {
        case WM_CREATE:
            // Создание полей ввода и кнопок
            createChildWindows(hwnd);
            return 0;
        case WM_COMMAND:
            // Обработка нажитий на кнопки
            if (wParam == ID_CREATE_BTN) {
                createProcess(hwnd, createText);
            } else if (wParam == ID_UPDATE_BTN) {
                updateProcess(hwnd, updateText);
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
