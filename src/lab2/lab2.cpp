#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "../../include/errorHandling.hpp"
#include "../../include/keymaps.hpp"

#define ID_BTN 0x1
#define ID_EDIT 0x2

// Определение функции задающей поведение окна
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR ptrCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"FormWindow";
    RegisterClass(&wc);

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
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    
    static WCHAR editTextBuf[512] = {};

    switch (uMsg) {
        case WM_CREATE: {
            CreateWindow(L"EDIT",  // Предопределенный класс
                         L"Enter command here",  // Текст кнопки
                         WS_VISIBLE | WS_CHILD,  // Стили окна
                         220, 100,               // Позиция
                         200, 100,               // Размер
                         hwnd,             // Родительское окно
                         (HMENU)ID_EDIT,  // Меню
                         (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),  // Дескриптор экземпляра
                         NULL);  // Дополнительные данные приложения

            CreateWindow(L"BUTTON",  // Предопределенный класс
                         L"Запустить",        // Текст кнопки
                         WS_VISIBLE | WS_CHILD,  // Стили окна
                         270, 200,               // Позиция
                         100, 50,                // Размер
                         hwnd,             // Родительское окно
                         (HMENU)ID_BTN,  // Меню
                         (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),  // Дескриптор экземпляра
                         NULL);  // Дополнительные данные приложения
        }
            return 0;

        case WM_DESTROY: {
            // Do nothing
        }
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND: {
            if (wParam == ID_BTN) {
                // Указание станцию окна, рабочий стол, стандартные дескрипторы и внешний вид главного окна для процесса во время создания.
                STARTUPINFO si = {};
                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);

                // Содержит информацию о недавно созданном процессе и его основном потоке.
                PROCESS_INFORMATION pi = {};
                ZeroMemory(&pi, sizeof(pi));

                // Создание пути к исполняемому файлу
                WCHAR path[1024] = {};
                DWORD pathLen = GetCurrentDirectory(1024, path);
                LPCWSTR localPath = L"\\builds\\lab1.exe";
                for (int i = 0; i < 17; i++) {
                    path[i + pathLen] = localPath[i];
                }
                path[pathLen + 17] = '\0';

                // Получение текста из поля ввода
                int editTextLen = SendDlgItemMessageW(hwnd, ID_EDIT, WM_GETTEXTLENGTH, 0, 0);
                GetDlgItemText(hwnd, ID_EDIT, editTextBuf, 512);
                editTextBuf[editTextLen] = '\0';

                // Запуск исполняемого файла первой программы
                BOOL win = CreateProcess(
                    path,  // Имя выполняемого модуля
                    NULL,  // Аргументы командной строки
                    NULL,  // Дескриптор процесса не наследуется
                    NULL,  // Дескриптор потока не наследуется
                    FALSE,  // Запрет наследования дескриптора
                    0,  // Нет флагов создания
                    NULL,  // Используйте родительский блок среды
                    NULL,  // Используйте рабочую дирикторию родителя
                    &si,  // Указатель на структуру STARTUPINFO
                    &pi);  // Указатель на структуру PROCESS_INFORMATION
                
                if (!win) {
                    ErrorExit(TEXT("CreateProcess"));
                    return 0;
                }

                // Подождите, пока дочерний процесс не создастся
                WaitForSingleObject(pi.hProcess, 1000);

            /// TODO: Сделать отправку команд на смену цвета фона
                HWND findHwnd = FindWindow(L"DynamicBackgroundWindow", L"Dynamic Background Window");
                if (!findHwnd) {
                    ErrorExit(TEXT("Find Window DynamicBackgroundWindow"));
                    return 0;
                }
                SendMessage(findHwnd, WM_KEYDOWN, Q_KEY, NULL);

                // Ждём пока дочерний процесс завершится
                WaitForSingleObject(pi.hProcess, 333);

                // Закрываем дескрипторы процессов и потоков
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        }  // case WM_COMMAND
            return 0;
    }  // Switch

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}  // WndProc
