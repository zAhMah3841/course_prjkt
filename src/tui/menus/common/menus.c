#include "../../utils/utils.h"
#include "../menus.h"

MenuParam createIntParam(const int value) {
    MenuParam param;
    param.int_val = value;
    return param;
}

MenuParam createDoubleParam(const double value) {
    MenuParam param;
    param.double_val = value;
    return param;
}

MenuParam createStringParam(const char* value) {
    MenuParam param;
    param.str_val = (char*)value;
    return param;
}

bool handleMenuChoice(const wint_t choice, const Menu* menu) {
    for (size_t i = 0; i < menu->count; i++) {
        if (choice == L'0') {
            wprintf(L"\nЗавершение работы...\n");
            return false;
        }

        if (choice == menu->items[i].key) {
            if (menu->items[i].handler)
                menu->items[i].handler(menu->items[i].param);

            return true;
        }
    }

    wprintf(L"\nНеверный выбор! Попробуйте снова.\n");
    waitForKey();
    return true;
}

void runMenu(const Menu* menu) {
    bool is_running = true;

    while (is_running) {
        clearScreen();
        wprintf(L"=== %ls ===\n\n", menu->title);

        for (size_t i = 0; i < menu->count; i++)
            wprintf(L"%lc. %ls\n", menu->items[i].key, menu->items[i].label);

        wprintf(L"\nВыберите пункт: ");
        wint_t choice = _getwch();
        wprintf(L"\n");

        is_running = handleMenuChoice(choice, menu);
    }
}

