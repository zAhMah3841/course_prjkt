#include "menus.h"

void runAdminMenu() {
    MenuItem items[] = {
        {L'1', L"Управление учетными записями", runAccountsManagerMenu, {0}},
        {L'2', L"Управление данными", runOrdersManagerMenu, {0}},
        {L'0', L"Выход", NULL, {0}}
    };

    const Menu adminMenu = {
        L"МЕНЮ АДМИНИСТРАТОРА",
        items,
        sizeof(items) / sizeof(items[0])
    };

    runMenu(&adminMenu);
}