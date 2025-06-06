#ifndef STYLE_DEFINITIONS_H
#define STYLE_DEFINITIONS_H

#include "colors_definitions.h"

// Символы для рамок
#define BORDER_TOP_LEFT     L"┌"
#define BORDER_TOP_RIGHT    L"┐"
#define BORDER_BOTTOM_LEFT  L"└"
#define BORDER_BOTTOM_RIGHT L"┘"
#define BORDER_HORIZONTAL   L"─"
#define BORDER_VERTICAL     L"│"

// Стили заголовков
#define HEADER_STYLE        ANSI_CYAN
#define HEADER_FORMAT       L"%ls"

// Стили подсказок и приглашений
#define PROMPT_STYLE        ANSI_BLUE
#define PROMPT_FORMAT       L"%ls"

// Стили сообщений об ошибках
#define ERROR_STYLE         ANSI_RED
#define ERROR_PREFIX        L"⚠ "
#define ERROR_FORMAT        ERROR_STYLE L"%ls" ANSI_RESET

// Стили сообщений об успехе
#define SUCCESS_STYLE       ANSI_GREEN
#define SUCCESS_PREFIX      L"✓ "
#define SUCCESS_FORMAT      SUCCESS_STYLE L"%ls" ANSI_RESET

// Стили меню
#define MENU_ITEM_STYLE     ANSI_WHITE
#define MENU_ITEM_FORMAT    L"%lc. %ls"

// Стили для ввода
#define INPUT_PROMPT_STYLE  ANSI_BRIGHT_BLUE
#define INPUT_PROMPT_FORMAT L"%ls"

// Стили для подсказок по навигации
#define NAV_HINT_STYLE      ANSI_BRIGHT_BLACK
#define NAV_HINT_FORMAT     L"%ls"

#endif // STYLE_DEFINITIONS_H 