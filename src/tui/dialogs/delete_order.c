#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../../database/orders_management/orders_management.h"
#include "../tables/tables.h"
#include "dialogs.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define DELETE_ORDER_TITLE ANSI_CYAN L"=== Удаление заказа ===" ANSI_RESET
#define ORDER_NUMBER_PROMPT ANSI_BLUE L"\nВведите номер заказа для удаления: " ANSI_RESET
#define CONFIRM_DELETE_MSG L"Вы действительно хотите удалить заказ №%d?"
#define SUCCESS_MSG ANSI_GREEN L"\nЗаказ успешно удален!\n" ANSI_RESET
#define ERROR_MSG ANSI_RED L"\nОшибка при удалении заказа!\n" ANSI_RESET
#define ORDER_NOT_FOUND_MSG ANSI_RED L"\nЗаказ с таким номером не найден!\n" ANSI_RESET
#define INVALID_NUMBER_MSG ANSI_RED L"\nНеверный формат номера!\n" ANSI_RESET

bool deleteOrderDialog() {
    wchar_t number_str[10] = {0};
    int number;

    while (true) {
        clearScreen();
        wprintf(L"%ls\n\n", DELETE_ORDER_TITLE);

        // Показываем список заказов
        printOrdersTable();

        // Запрашиваем номер для удаления
        if (!getInput(ORDER_NUMBER_PROMPT, number_str, 10)) {
            return false;
        }

        // Преобразуем строку в число
        wchar_t* end;
        number = wcstol(number_str, &end, 10);
        if (*end != L'\0') {
            wprintf(INVALID_NUMBER_MSG);
            waitForKey();
            continue;
        }

        // Проверяем существование заказа
        if (!isOrderExists(number)) {
            wprintf(ORDER_NOT_FOUND_MSG);
            waitForKey();
            continue;
        }

        // Запрашиваем подтверждение
        wchar_t confirm_msg[256];
        swprintf(confirm_msg, 256, CONFIRM_DELETE_MSG, number);
        
        if (!confirmAction(confirm_msg, false)) {
            return false;
        }

        // Удаляем заказ
        if (deleteOrder(number)) {
            wprintf(SUCCESS_MSG);
        } else {
            wprintf(ERROR_MSG);
        }

        waitForKey();
        return true;
    }
} 