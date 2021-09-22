#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wchar.h>

#include "keymaps.hpp"
#include "colors.hpp"
#include "errorHandling.hpp"

#define WM_MYCASE 0xFFFF

#define ID_CREATE_BTN  0x01
#define ID_CREATE_EDIT 0x02

#define ID_UPDATE_BTN  0x11
#define ID_UPDATE_EDIT 0x12

void drawText(const HWND &hwnd, const LPWSTR &text, const COLORREF &color) {
    PAINTSTRUCT pStruct = {};
    HDC hDeviceContext = BeginPaint(hwnd, &pStruct);

    RECT rectPlace = {};
    GetClientRect(hwnd, &rectPlace);

    // Закрашивание фона
    HBRUSH hBrush = CreateSolidBrush(color);
    SelectObject(hDeviceContext, hBrush);
    FillRect(hDeviceContext, &rectPlace, hBrush);

    // Отрисовка текста
    HFONT handleFont = CreateFont(90, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial Bold");
    SelectObject(hDeviceContext, handleFont);
    SetBkColor(hDeviceContext, color);
    SetTextColor(hDeviceContext, BLACK);
    DrawText(hDeviceContext, text, 1, &rectPlace, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    EndPaint(hwnd, &pStruct);
}

// Преобразование строки с названием цвета в COLORREF значение
void getColorFromStr(const LPWSTR text, COLORREF &color) {
    if (wcscmp(text, L"RED") == 0) {
        color = RED;
    } else if (wcscmp(text, L"GREEN") == 0) {
        color = GREEN;
    } else if (wcscmp(text, L"BLUE") == 0) {
        color = BLUE;
    } else if (wcscmp(text, L"CYAN") == 0) {
        color = CYAN;
    } else if (wcscmp(text, L"MAGENTA") == 0) {
        color = MAGENTA;
    } else if (wcscmp(text, L"YELLOW") == 0) {
        color = YELLOW;
    }
}

// Обработка нажатых клавиш
void keyHandler(const HWND &hwnd, const WPARAM &wParam, const LPWSTR text, COLORREF &color) {
    // Смена отображаемого символа только если нажата кнопка с буквой латинского алфавита
    if (wParam > A_KEY && wParam < Z_KEY) {
        text[0] = (WCHAR)wParam;
    }
    switch (wParam) {
        case Q_KEY:
            color = CYAN;
            break;
        case W_KEY:
            color = MAGENTA;
            break;
        case E_KEY:
            color = ORANGE;
            break;
        case R_KEY:
            color = RED;
            break;
        case G_KEY:
            color = GREEN;
            break;
        case B_KEY:
            color = BLUE;
            break;
        case VK_ESCAPE:
        case VK_BACK:
            PostQuitMessage(0);
            break;
        default:
            break;
    }  // Case
    InvalidateRect(hwnd, NULL, TRUE);
}  // keyHandler

// Создание полей ввода и кнопок
void createChildWindows(const HWND &hwnd){
    // Создание поля ввода для параметра запуска
    CreateWindow(L"EDIT",  // Предопределенный класс
                L"Enter color here",    // Текст кнопки
                WS_VISIBLE | WS_CHILD,  // Стили окна
                20, 100,                // Позиция
                200, 100,               // Размер
                hwnd,  // Родительское окно
                (HMENU)ID_CREATE_EDIT,  // Идентификатор элемента в меню
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),  // Дескриптор экземпляра
                NULL);  // Дополнительные данные приложения

    // Создание кнопки для запуска
    CreateWindow(L"BUTTON",
                L"Start new",
                WS_VISIBLE | WS_CHILD,
                70, 200,
                100, 50,
                hwnd,
                (HMENU)ID_CREATE_BTN,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL);

    // Создание поля для ввода названия цвета для его изменения
    CreateWindow(L"EDIT",
                L"Enter color here",
                WS_VISIBLE | WS_CHILD,
                320, 100,
                200, 100,
                hwnd,
                (HMENU)ID_UPDATE_EDIT,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL);

    // Создание поля для изменения цвета
    CreateWindow(L"BUTTON",
                L"Update color",
                WS_VISIBLE | WS_CHILD,
                370, 200,
                100, 50,
                hwnd,
                (HMENU)ID_UPDATE_BTN,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL);
}

// Создание процесса и задание ему фона
void createProcess(const HWND &hwnd, LPWSTR createText) {
    // Указание станцию окна, рабочий стол, стандартные дескрипторы и внешний вид главного окна для процесса во время создания.
    STARTUPINFO si = {};
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Содержит информацию о недавно созданном процессе и его основном потоке.
    PROCESS_INFORMATION pi = {};
    ZeroMemory(&pi, sizeof(pi));

    // Создание пути к исполняемому файлу
    WCHAR path[1024] = {};
    DWORD pathLen = GetCurrentDirectory(1006, path);
    LPCWSTR localPath = L"\\builds\\lab1.exe";
    for (int i = 0; i < 17; i++) {
        path[i + pathLen] = localPath[i];
    }
    path[pathLen + 17] = '\0';

    // Получение текста из поля ввода
    int createTextLen = SendDlgItemMessage(hwnd, ID_CREATE_EDIT, WM_GETTEXTLENGTH, 0, 0);
    GetDlgItemText(hwnd, ID_CREATE_EDIT, createText, 512);
    createText[createTextLen] = '\0';

    // Запуск исполняемого файла первой программы
    BOOL created = CreateProcess(path,  // Имя выполняемого модуля
                                createText,  // Аргументы командной строки
                                NULL,  // Дескриптор процесса не наследуется
                                NULL,  // Дескриптор потока не наследуется
                                FALSE,  // Запрет наследования дескриптора
                                0,  // Нет флагов создания
                                NULL,  // Используйте родительский блок среды
                                NULL,  // Используйте рабочую дирикторию родителя
                                &si,  // Указатель на структуру STARTUPINFO
                                &pi);  // Указатель на структуру PROCESS_INFORMATION
    if (!created) {
        ErrorExit(TEXT("CreateProcess"));
    }
}

// Изменение цвета фона окна запущенного процесса
void updateProcess(const HWND &hwnd, LPWSTR updateText) {
    // Получение текста из поля ввода
    int updateTextLen = SendDlgItemMessage(hwnd, ID_UPDATE_EDIT, WM_GETTEXTLENGTH, 0, 0);
    GetDlgItemText(hwnd, ID_UPDATE_EDIT, updateText, 512);
    updateText[updateTextLen] = '\0';

    // Поиск окна запущенного процесса
    HWND findHwnd =
        FindWindow(L"DynamicBackgroundWindow", L"Dynamic Background Window");
    if (!findHwnd) {
        ErrorExit(TEXT("Find Window DynamicBackgroundWindow"));
    }

    // Преобразование строки с названием цвета в COLORREF значение
    COLORREF color = {};
    getColorFromStr(updateText, color);

    // Отправка собщения на смену цвета
    SendMessage(findHwnd, WM_MYCASE, color, NULL);
}
