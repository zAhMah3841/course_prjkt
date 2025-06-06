#include "menus.h"
#include "../tables/tables.h"
#include "../dialogs/dialogs.h"

void viewAllOrdersWrapper(MenuParam param) {
    printOrdersTableAndWait();
}

void searchOrdersWrapper(MenuParam param) {
    searchOrdersDialog();
}

void sortOrdersWrapper(MenuParam param) {
    sortOrdersDialog();
}

void runUserMenu() {
    MenuItem items[] = {
        {L'1', L"Просмотр всех заявок", viewAllOrdersWrapper, {0}},
        {L'2', L"Поиск заявок", searchOrdersWrapper, {0}},
        {L'3', L"Сортировка заявок", sortOrdersWrapper, {0}},
        {L'0', L"Выход", NULL, {0}}
    };

    const Menu userMenu = {
        L"МЕНЮ ПОЛЬЗОВАТЕЛЯ",
        items,
        sizeof(items) / sizeof(items[0])
    };

    runMenu(&userMenu);
}