#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../../database/orders_management/orders_management.h"
#include "../widgets/table/table.h"
#include "../tables/tables.h"
#include "dialogs.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <stdbool.h>

#define SEARCH_TITLE ANSI_CYAN L"=== Поиск заказчиков по первой букве имени ===" ANSI_RESET
#define LETTER_PROMPT ANSI_BLUE L"\nВведите первую букву имени: " ANSI_RESET
#define NO_RESULTS_MSG ANSI_YELLOW L"\nЗаказчиков с именем на букву '%lc' не найдено.\n" ANSI_RESET
#define RESULTS_FOUND_MSG ANSI_GREEN L"\nНайдены следующие заказы:\n" ANSI_RESET
#define INVALID_INPUT_MSG ANSI_RED L"\nПожалуйста, введите ровно одну букву.\n" ANSI_RESET

// Вспомогательная функция для отображения найденных заказов
void printFoundOrders(Order* orders, size_t count) {
    if (!orders || count == 0) return;

    // Создаем таблицу
    Table* table = createTable(count + 1, 7); // +1 для заголовка

    // Устанавливаем заголовки
    setCell(table, 0, 0, L"Номер");
    setCell(table, 0, 1, L"Фамилия");
    setCell(table, 0, 2, L"Имя");
    setCell(table, 0, 3, L"Отчество");
    setCell(table, 0, 4, L"Тип работы");
    setCell(table, 0, 5, L"Стоимость");
    setCell(table, 0, 6, L"Команда");

    // Заполняем данные
    for (size_t i = 0; i < count; i++) {
        // Конвертируем номер в строку
        wchar_t number[32];
        swprintf(number, 32, L"%d", orders[i].number);
        setCell(table, i + 1, 0, number);

        // Устанавливаем строковые значения
        setCell(table, i + 1, 1, orders[i].last_name);
        setCell(table, i + 1, 2, orders[i].first_name);
        setCell(table, i + 1, 3, orders[i].surname_name);
        setCell(table, i + 1, 4, orders[i].work_type);

        // Конвертируем цену в строку
        wchar_t price[32];
        swprintf(price, 32, L"%.2f", orders[i].price);
        setCell(table, i + 1, 5, price);

        setCell(table, i + 1, 6, orders[i].team);
    }

    // Настройка форматирования
    for (int i = 0; i < 7; i++) {
        table->col_wrap[i] = true;
    }
    table->col_wrap[0] = false; // Номер не переносим
    table->col_wrap[5] = false; // Цену не переносим

    // Выводим таблицу
    calculateLayout(table, 120);
    printTable(table);

    // Освобождаем память таблицы
    freeTable(table);
}

bool searchOrdersByLetterDialog() {
    wchar_t input[10] = {0};
    wchar_t letter;

    while (true) {
        clearScreen();
        wprintf(L"%ls\n\n", SEARCH_TITLE);

        // Запрашиваем букву
        if (!getInput(LETTER_PROMPT, input, 10)) {
            return false;
        }

        // Проверяем, что введена ровно одна буква
        if (wcslen(input) != 1) {
            wprintf(INVALID_INPUT_MSG);
            waitForKey();
            continue;
        }

        letter = input[0];

        // Ищем заказы
        size_t count;
        Order* found_orders = findOrdersByFirstLetter(letter, &count);

        if (!found_orders || count == 0) {
            wprintf(NO_RESULTS_MSG, letter);
        } else {
            wprintf(RESULTS_FOUND_MSG);
            printFoundOrders(found_orders, count);

            // Освобождаем память найденных заказов
            for (size_t i = 0; i < count; i++) {
                free(found_orders[i].last_name);
                free(found_orders[i].first_name);
                free(found_orders[i].surname_name);
                free(found_orders[i].work_type);
                free(found_orders[i].team);
            }
            free(found_orders);
        }

        waitForKey();
        return true;
    }
} 