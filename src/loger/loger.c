#include "loger.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

static wchar_t* ansiToWide(const char* str) {
    if (!str) return NULL;
    int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    wchar_t* wstr = (wchar_t*)malloc(size * sizeof(wchar_t));
    if (wstr) {
        MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, size);
    }
    return wstr;
}

void loggerLog(LogLevel level, const char* message) {
    wchar_t* wmsg = ansiToWide(message);
    if (wmsg) {
        loggerLogW(level, wmsg);
        free(wmsg);
    } else {
        fprintf(stderr, "[FALLBACK] %s\n", message);
    }
}

void loggerLogW(LogLevel level, const wchar_t* message) {
    const wchar_t* prefix = L"";
    switch (level) {
        case LOG_LEVEL_ERROR: prefix = L"[ERROR] "; break;
        case LOG_LEVEL_WARNING: prefix = L"[WARNING] "; break;
        case LOG_LEVEL_INFO: prefix = L"[INFO] "; break;
        case LOG_LEVEL_DEBUG: prefix = L"[DEBUG] "; break;
    }
    fwprintf(stderr, L"%ls%ls\n", prefix, message);
}