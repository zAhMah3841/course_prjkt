#include "layout_utils.h"
#include "style_definitions.h"
#include <stdio.h>

ConsoleSize getConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    ConsoleSize size = {80, 24}; // Значения по умолчанию

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        size.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        size.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return size;
}

void printCentered(const wchar_t* text, int width) {
    size_t text_len = wcslen(text);
    int padding = (width - text_len) / 2;
    if (padding > 0) {
        for (int i = 0; i < padding; i++) {
            wprintf(L" ");
        }
    }
    wprintf(L"%ls", text);
}

BoxBounds createCenteredBox(int width, int height) {
    ConsoleSize console = getConsoleSize();
    BoxBounds bounds;
    
    bounds.width = (width > 0) ? width : console.width - 4;
    bounds.height = (height > 0) ? height : console.height - 4;
    bounds.x = (console.width - bounds.width) / 2;
    bounds.y = (console.height - bounds.height) / 2;
    
    return bounds;
}

void drawBox(BoxBounds bounds) {
    // Верхняя граница
    setCursorPosition(bounds.x, bounds.y);
    wprintf(BORDER_TOP_LEFT);
    for (int i = 0; i < bounds.width - 2; i++) {
        wprintf(BORDER_HORIZONTAL);
    }
    wprintf(BORDER_TOP_RIGHT);

    // Боковые границы
    for (int i = 1; i < bounds.height - 1; i++) {
        setCursorPosition(bounds.x, bounds.y + i);
        wprintf(BORDER_VERTICAL);
        setCursorPosition(bounds.x + bounds.width - 1, bounds.y + i);
        wprintf(BORDER_VERTICAL);
    }

    // Нижняя граница
    setCursorPosition(bounds.x, bounds.y + bounds.height - 1);
    wprintf(BORDER_BOTTOM_LEFT);
    for (int i = 0; i < bounds.width - 2; i++) {
        wprintf(BORDER_HORIZONTAL);
    }
    wprintf(BORDER_BOTTOM_RIGHT);
}

void drawHorizontalLine(int x, int y, int width) {
    setCursorPosition(x, y);
    wprintf(BORDER_VERTICAL L" ");
    for (int i = 0; i < width - 6; i++) {
        wprintf(BORDER_HORIZONTAL);
    }
    wprintf(L" " BORDER_VERTICAL);
}

void drawBoxedHeader(const wchar_t* header) {
    ConsoleSize console = getConsoleSize();
    int header_width = wcslen(header) + 4;
    int x = (console.width - header_width) / 2;
    
    // Верхняя граница с заголовком
    setCursorPosition(x, 1);
    wprintf(HEADER_STYLE);
    wprintf(BORDER_TOP_LEFT BORDER_HORIZONTAL L" %ls " BORDER_HORIZONTAL BORDER_TOP_RIGHT, header);
    wprintf(ANSI_RESET);
}

void drawInputField(const wchar_t* label, int y_position) {
    ConsoleSize console = getConsoleSize();
    int field_width = console.width - 8;
    int x = 4;
    
    setCursorPosition(x, y_position);
    wprintf(INPUT_PROMPT_STYLE L"%ls: " ANSI_RESET, label);
}

void drawSeparator(int y_position) {
    ConsoleSize console = getConsoleSize();
    int x = 2;
    int width = console.width - 4;
    
    setCursorPosition(x, y_position);
    wprintf(ANSI_BRIGHT_BLACK);
    drawHorizontalLine(x, y_position, width);
    wprintf(ANSI_RESET);
}

void setCursorPosition(int x, int y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hOut, pos);
} 