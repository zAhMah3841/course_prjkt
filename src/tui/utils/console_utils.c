#include "colors_definitions.h"
#include <windows.h>
#include <locale.h>
#include <fcntl.h>
#include "utils.h"
#include <wchar.h>
#include <stdio.h>
#include <io.h>

#define PRESS_KEY_MSG ANSI_MAGENTA L"\nНажмите любую клавишу для продолжения..." ANSI_RESET

volatile sig_atomic_t exit_flag = 0;

void initConsole() {
    // Установка кодировки UTF-8 для ввода/вывода
    SetConsoleOutputCP(65001);  // UTF-8
    SetConsoleCP(65001);        // UTF-8

    // Включение обработки виртуального терминала для escape-последовательностей ANSI
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
    SetConsoleMode(hOut, dwMode);

    // Установка локали для поддержки широких символов
    setlocale(LC_ALL, ".UTF-8");
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    //
    CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
    if (GetCurrentConsoleFontEx(hOut, FALSE, &fontInfo)) {
        wcscpy(fontInfo.FaceName, L"NSimSun");  // NSimSun has good support for box-drawing characters
        fontInfo.dwFontSize.Y = 16;
        SetCurrentConsoleFontEx(hOut, FALSE, &fontInfo);
    }
}


void handleSignal(int sig) {
    exit_flag = 1;
}

bool checkForExit() {
    if (_kbhit()) {
        wint_t ch = _getwch();
        if (ch == 27) {
            return true;
        }
    }
    return false;
}

void clearScreen() {
    wprintf(L"\033[2J\033[H");
}

void waitForKey() {
    wprintf(PRESS_KEY_MSG);
    while(_getwch() == 0);
}

bool getInput(const wchar_t* prompt, wchar_t* buffer, size_t size) {
    wprintf(L"%ls", prompt);

    size_t i = 0;
    while (true) {
        if (exit_flag) return false;
        wint_t ch = _getwch();

        if (ch == 27) return false;
        if (ch == L'\r') { // Enter
            buffer[i] = L'\0';
            wprintf(L"\n");
            return true;
        }

        if (ch == L'\b' && i > 0) {
            i--;
            wprintf(L"\b \b");
        } else if (i < size - 1 && ch >= 32) {
            buffer[i++] = ch;
            wprintf(L"%lc", ch);
        }
    }
}

bool getPassword(const wchar_t* prompt, wchar_t* buffer, size_t size) {
    wprintf(L"%ls", prompt);

    size_t i = 0;
    while (true) {
        if (exit_flag) return false;

        wint_t ch = _getwch();
        if (ch == 27) return false;

        if (ch == L'\r') {
            buffer[i] = L'\0';
            wprintf(L"\n");
            return true;
        }

        if (ch == L'\b' && i > 0) {
            i--;
            wprintf(L"\b \b");
        }
        else if (i < size - 1 && ch >= 32) {
            buffer[i++] = ch;
            wprintf(L"*");
        }
    }
}

char* wcharToUtf8(const wchar_t* wstr) {
    if (!wstr) return NULL;
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = (char*)malloc(size);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, size, NULL, NULL);
    return str;
}

char* wideToUtf8(const wchar_t* wideStr) {
    if (!wideStr) return NULL;

    int size = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL);
    if (size == 0) return NULL;

    char* utf8Str = (char*)malloc(size);
    if (!utf8Str) return NULL;

    if (WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str, size, NULL, NULL) == 0) {
        free(utf8Str);
        return NULL;
    }

    return utf8Str;
}

wchar_t* utf8ToWide(const char* utf8Str) {
    if (!utf8Str) return NULL;

    int wideSize = MultiByteToWideChar(CP_UTF8, 0, utf8Str,-1, NULL,0);
    if (wideSize == 0) return NULL;

    wchar_t* wideStr = (wchar_t*)malloc(wideSize * sizeof(wchar_t));
    if (!wideStr) return NULL;

    if (MultiByteToWideChar(CP_UTF8,0, utf8Str,-1, wideStr,wideSize) == 0) {
        free(wideStr);
        return NULL;
    }

    return wideStr;
}

size_t wstrWidth(const wchar_t* str) {
    if (!str) return 0;

    size_t width = 0;
    while (*str) {
        width += (*str == L'\n' ? 0 : 1);
        str++;
    }
    return width;
}

wchar_t* trimWhitespace(wchar_t* str) {
    if (!str) return NULL;
    while (iswspace(*str)) str++;

    if (*str == L'\0') return str;

    wchar_t* end = str + wcslen(str) - 1;
    while (end > str && iswspace(*end)) {
        end--;
    }

    *(end + 1) = L'\0';
    return str;
}

bool confirmAction(const wchar_t* message, bool default_choice) {
    wchar_t prompt[256];
    const wchar_t* options = default_choice ? L"[Y/n]" : L"[y/N]";

    swprintf(prompt, sizeof(prompt)/sizeof(wchar_t), L"%ls %ls ", message, options);
    wprintf(L"%ls", prompt);

    wint_t ch = _getwch();
    wprintf(L"\n");

    switch (ch) {
        case L'y': case L'Y': case L'д': case L'Д':
            return true;
        case L'n': case L'N': case L'н': case L'Н':
            return false;
        case L'\r':
            return default_choice;
        case 27:
            return false;
        default:
            wprintf(ANSI_YELLOW L"Пожалуйста, введите 'y' (да) или 'n' (нет)\n" ANSI_RESET);
            return confirmAction(message, default_choice);
    }
}