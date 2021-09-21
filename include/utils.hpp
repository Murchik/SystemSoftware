#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "keymaps.hpp"
#include "colors.hpp"

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

void keyHandler(const HWND &hwnd, const WPARAM &wParam, const LPWSTR &text, COLORREF &color) {
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
