#ifndef LAYOUT_UTILS_H
#define LAYOUT_UTILS_H

#include <windows.h>

typedef struct {
    int width;
    int height;
} ConsoleSize;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} BoxBounds;

// Получение размеров консоли
ConsoleSize getConsoleSize();

// Центрирование текста
void printCentered(const wchar_t* text, int width);

// Создание рамки с учетом размеров
BoxBounds createCenteredBox(int width, int height);

// Отрисовка рамки
void drawBox(BoxBounds bounds);

// Отрисовка горизонтальной линии
void drawHorizontalLine(int x, int y, int width);

// Отрисовка заголовка в рамке
void drawBoxedHeader(const wchar_t* header);

// Отрисовка поля ввода
void drawInputField(const wchar_t* label, int y_position);

// Отрисовка разделителя
void drawSeparator(int y_position);

// Перемещение курсора
void setCursorPosition(int x, int y);

#endif // LAYOUT_UTILS_H 