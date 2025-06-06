#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../../database/orders_management/orders_management.h"
#include "../tables/tables.h"
#include "dialogs.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define CHECK_NUMBER_TITLE ANSI_CYAN L"=== Проверка уникальности номера заказа ===" ANSI_RESET
#define ORDER_NUMBER_PROMPT ANSI_BLUE L"\nВведите номер заказа для проверки: " ANSI_RESET
#define NUMBER_EXISTS_MSG ANSI_RED L"\nЗаказ с номером %d уже существует!\n" ANSI_RESET
#define NUMBER_UNIQUE_MSG ANSI_GREEN L"\nНомер %d свободен и может быть использован.\n" ANSI_RESET
#define INVALID_NUMBER_MSG ANSI_RED L"\nНеверный формат номера!\n" ANSI_RESET

bool checkOrderNumberDialog() {
    wchar_t number_str[10] = {0};
    int number;

    while (true) {
        clearScreen();
        wprintf(L"%ls\n\n", CHECK_NUMBER_TITLE);

        // Показываем список существующих заказов для справки
        printOrdersTable();

        // Запрашиваем номер для проверки
        if (!getInput(ORDER_NUMBER_PROMPT, number_str, 10)) {
            return false;
        }

        // Преобразуем строку в число
        wchar_t* end;
        number = wcstol(number_str, &end, 10);
        if (*end != L'\0' || number <= 0) {
            wprintf(INVALID_NUMBER_MSG);
            waitForKey();
            continue;
        }

        // Проверяем существование заказа
        if (isOrderExists(number)) {
            wprintf(NUMBER_EXISTS_MSG, number);
        } else {
            wprintf(NUMBER_UNIQUE_MSG, number);
        }

        waitForKey();
        return true;
    }
} 