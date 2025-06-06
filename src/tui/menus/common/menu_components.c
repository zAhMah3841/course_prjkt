#include "menu_components.h"
#include "../../utils/style_definitions.h"
#include <wchar.h>

// Определение предопределенных стилей
const MenuStyle MAIN_MENU_STYLE = {
    L"ГЛАВНОЕ МЕНЮ",
    40,    // min_width
    4,     // padding_x
    2,     // padding_y
    1      // spacing
};

const MenuStyle ADMIN_MENU_STYLE = {
    L"МЕНЮ АДМИНИСТРАТОРА",
    50,    // min_width
    4,     // padding_x
    2,     // padding_y
    1      // spacing
};

const MenuStyle USER_MENU_STYLE = {
    L"МЕНЮ ПОЛЬЗОВАТЕЛЯ",
    45,    // min_width
    4,     // padding_x
    2,     // padding_y
    1      // spacing
};

const MenuStyle MANAGER_MENU_STYLE = {
    L"УПРАВЛЕНИЕ",
    55,    // min_width
    4,     // padding_x
    2,     // padding_y
    1      // spacing
};

BoxBounds calculateMenuBounds(const Menu* menu, const MenuStyle* style) {
    ConsoleSize console = getConsoleSize();
    int required_height = style->padding_y * 2 + menu->count + (menu->count - 1) * style->spacing + 4;
    int required_width = style->min_width;
    
    // Находим самый длинный пункт меню
    for (size_t i = 0; i < menu->count; i++) {
        int item_length = wcslen(menu->items[i].label) + 4; // +4 для номера и отступов
        if (item_length > required_width) {
            required_width = item_length + style->padding_x * 2;
        }
    }
    
    return createCenteredBox(required_width, required_height);
}

void drawMenuTitle(const wchar_t* title, const MenuStyle* style, BoxBounds* bounds) {
    wchar_t full_title[256];
    if (style->title_prefix) {
        swprintf(full_title, 256, L"%ls: %ls", style->title_prefix, title);
    } else {
        wcscpy(full_title, title);
    }
    
    setCursorPosition(bounds->x + (bounds->width - wcslen(full_title)) / 2 - 2, bounds->y + style->padding_y);
    wprintf(HEADER_STYLE L"[ %ls ]" ANSI_RESET, full_title);
}

void drawMenuItem(const MenuItem* item, int index, const MenuStyle* style, BoxBounds* bounds) {
    int y = bounds->y + style->padding_y + 2 + index * (1 + style->spacing);
    
    if (item->key == L'0') {
        drawSeparator(y - 1);
    }
    
    setCursorPosition(bounds->x + style->padding_x, y);
    if (item->key == L'0') {
        wprintf(NAV_HINT_STYLE L"%lc. %ls" ANSI_RESET, item->key, item->label);
    } else {
        wprintf(MENU_ITEM_STYLE L"%lc. %ls" ANSI_RESET, item->key, item->label);
    }
}

void drawMenuFooter(const MenuStyle* style, BoxBounds* bounds) {
    int y = bounds->y + bounds->height - style->padding_y - 1;
    drawSeparator(y);
}

void drawMenuPrompt(const wchar_t* prompt, const MenuStyle* style, BoxBounds* bounds) {
    setCursorPosition(bounds->x + style->padding_x, bounds->y + bounds->height - style->padding_y);
    wprintf(INPUT_PROMPT_STYLE L"%ls" ANSI_RESET, prompt);
} 