#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdio.h>

#include "keymaps.hpp"
#include "colors.hpp"

void paint(const HWND &hwnd, const LPWSTR &text, const COLORREF &color)
{
    PAINTSTRUCT pStruct = {};
    RECT rectPlace = {};
    HDC hDeviceContext = {};
    HFONT handleFont = CreateFont(90,              // Высота, в логических единицах, символьной ячейки или символа шрифта. Значение высоты символа (также известное как высота em) - это значение высоты ячейки символа минус начальное значение.
                                  0,               // Средняя ширина символов запрашиваемого шрифта в логических единицах. Если это значение равно нулю, сопоставитель шрифтов выбирает наиболее близкое значение соответствия.
                                  0,               // Угол в десятых долях градуса между вектором спуска и осью x устройства. Вектор спуска параллелен базовой строке строки текста.
                                  0,               // Угол в десятых долях градуса между базовой линией каждого символа и осью x устройства
                                  0,               // Вес шрифта в диапазоне от 0 до 1000. (Например, 400 - это нормально, а 700 - жирный шрифт)
                                  0,               // Указывает курсивный шрифт, если установлено значение TRUE
                                  0,               // Указывает подчёркнутый шрифт, если установлено значение TRUE
                                  0,               // Указывает зачёркнутый шрифт, если установлено значение TRUE
                                  DEFAULT_CHARSET, // Задание набора символов
                                  0,               // Точность вывода. Определяет, насколько точно выходные данные должны соответствовать высоте, ширине, ориентации символов, высоте, высоте и типу шрифта запрашиваемого шрифта.
                                  0,               // Точность отсечения. Определяет, как обрезать символы, которые частично находятся за пределами области отсечения.
                                  0,               // Качество вывода. Определяет, насколько тщательно GDI должен пытаться сопоставить атрибуты логического шрифта с атрибутами фактического физического шрифта.
                                  0,               // Высота и семейство шрифта.
                                  L"Arial Bold"    // Имя шрифта.
    );

    hDeviceContext = BeginPaint(hwnd, &pStruct);
    GetClientRect(hwnd, &rectPlace);
    SetTextColor(hDeviceContext, BLACK);
    SetBkColor(hDeviceContext, color);
    SelectObject(hDeviceContext, handleFont);
    DrawText(hDeviceContext, text, 1, &rectPlace,
             DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    EndPaint(hwnd, &pStruct);
}

void changeBackgroundColor(const HWND &hwnd, const COLORREF &color)
{
    HBRUSH hBrush;
    hBrush = CreateSolidBrush(color);
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
}

void keyHandler(const HWND &hwnd, const WPARAM &wParam, const LPWSTR &text, COLORREF &color)
{
    switch (wParam)
    {
    case Q_KEY:
        text[0] = (char)wParam;
        color = CYAN;
        changeBackgroundColor(hwnd, color);
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case W_KEY:
        text[0] = (char)wParam;
        color = MAGENTA;
        changeBackgroundColor(hwnd, color);
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case E_KEY:
        text[0] = (char)wParam;
        color = ORANGE;
        changeBackgroundColor(hwnd, color);
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case R_KEY:
        text[0] = (char)wParam;
        color = RED;
        changeBackgroundColor(hwnd, color);
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case G_KEY:
        text[0] = (char)wParam;
        color = GREEN;
        changeBackgroundColor(hwnd, color);
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case B_KEY:
        text[0] = (char)wParam;
        color = BLUE;
        changeBackgroundColor(hwnd, color);
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_LEFT:
    case VK_RIGHT:
    case VK_UP:
    case VK_DOWN:
    case VK_DELETE:
    case VK_SHIFT:
    case VK_SPACE:
    case VK_CONTROL:
    case VK_CAPITAL:
    case VK_MENU:
    case VK_TAB:
    case VK_BACK:
    case VK_RETURN:
        break;
    default:
        text[0] = (char)wParam;
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
}
