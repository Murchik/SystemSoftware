#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "../../include/keymaps.hpp"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR ptrCmdLine,
                   int nCmdShow)
{
    // Регистрация класса окна
    const wchar_t className[] = L"ButtonWindow";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(0,                   // Дополнительные стили окон
                               className,           // Класс окна
                               L"Lab2",             // Текст окна
                               WS_OVERLAPPEDWINDOW, // Стиль окна

                               // Размер и позиция
                               CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,

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

    HWND hwndButton = CreateWindow(L"BUTTON",       // Предопределенный класс
                                   L"Сделать лабу", // Текст кнопки
                                   // Стили окна
                                   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,

                                   // Позиция x и y и ширина и высота окна
                                   100, 100, 200, 100,

                                   hwnd,                                              // Родительское окно
                                   NULL,                                              // Меню
                                   (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), // Дескриптор экземпляра
                                   NULL);                                             // Дополнительные данные приложения
    if (!hwndButton)
    {
        MessageBox(NULL, L"Не удалось создать окно с кнопкой!", L"Выход", MB_OK);
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
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
    {
        // Указание станцию окна, рабочий стол, стандартные дескрипторы и внешний вид главного окна для процесса во время создания.
        STARTUPINFO si = {};
        // Содержит информацию о недавно созданном процессе и его основном потоке.
        PROCESS_INFORMATION pi = {};

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        LPCWSTR path = L"D:\\Libraries\\Documents\\Repos\\SystemSoftware\\builds\\lab1.exe";
        if (!CreateProcess(path,  // Имя выполняемого модуля
                           NULL,  // Аргументы командной строки
                           NULL,  // Дескриптор процесса не наследуется
                           NULL,  // Дескриптор потока не наследуется
                           FALSE, // Запрет наследования дескриптора
                           0,     // Нет флагов создания
                           NULL,  // Используйте родительский блок среды
                           NULL,  // Используйте рабочую дирикторию родителя
                           &si,   // Указатель на структуру STARTUPINFO
                           &pi)   // Указатель на структуру PROCESS_INFORMATION
        )
        {
            MessageBox(NULL, L"Не удалось создать процесс!", L"Ошибка", MB_OK);
            PostQuitMessage(0);
            return 0;
        }

        // Подождите, пока дочерний процесс не создастся
        WaitForSingleObject(pi.hProcess, 1000);

        HWND hwndHelp = FindWindow(L"WindowTemplate", L"Dynamic Background Window");
        if (!hwndHelp)
        {
            // Закрываем дескрипторы процессов и потоков
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            MessageBox(NULL, L"Созданное окно не найдено!", L"Ошибка", MB_OK);
            PostQuitMessage(0);
            return 0;
        }

        /// TODO: Сделать поведение первой проги по варианту
        SendMessage(hwndHelp, WM_KEYDOWN, Q_KEY, NULL);

        // Ждём пока дочерний процесс завершится
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Закрываем дескрипторы процессов и потоков
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
