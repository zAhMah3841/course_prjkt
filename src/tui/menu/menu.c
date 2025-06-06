#include "menu.h"
#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../dialogs/dialogs.h"
#include <stdio.h>
#include <wchar.h>

#define MENU_TITLE ANSI_CYAN L"=== Главное меню ===" ANSI_RESET

void showMenu() {
    while (true) {
        clearScreen();
        wprintf(L"%ls\n\n", MENU_TITLE);
        
        wprintf(L"1. Добавить заказ\n");
        wprintf(L"2. Редактировать заказ\n");
        wprintf(L"3. Удалить заказ\n");
        wprintf(L"4. Поиск заказов\n");
        wprintf(L"5. Поиск по первой букве имени\n");
        wprintf(L"0. Выход\n\n");
        wprintf(L"Выберите действие: ");

        wint_t choice = _getwch();
        wprintf(L"%lc\n", choice);

        switch (choice) {
            case L'1':
                addOrderDialog();
                break;
            case L'2':
                editOrderDialog();
                break;
            case L'3':
                deleteOrderDialog();
                break;
            case L'4':
                searchOrdersDialog();
                break;
            case L'5':
                searchOrdersByLetterDialog();
                break;
            case L'0':
                return;
            default:
                wprintf(L"\nНеверный выбор!\n");
                waitForKey();
        }
    }
} 