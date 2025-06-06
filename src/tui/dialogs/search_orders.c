#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../utils/order_validation.h"
#include "../../database/orders_management/orders_management.h"
#include "../widgets/table/table.h"
#include "../tables/tables.h"
#include "dialogs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <stdbool.h>

#define SEARCH_TITLE ANSI_CYAN L"=== Поиск заказов ===" ANSI_RESET
#define SEARCH_MENU_TITLE L"\nВыберите критерий поиска:"
#define NO_RESULTS_MSG ANSI_YELLOW L"\nЗаказы не найдены.\n" ANSI_RESET
#define RESULTS_FOUND_MSG ANSI_GREEN L"\nНайдены следующие заказы:\n" ANSI_RESET
#define INVALID_PRICE_INPUT_MSG ANSI_RED L"\nНеверный формат стоимости! Введите положительное число.\n" ANSI_RESET

#define LAST_NAME_PROMPT ANSI_BLUE L"\nВведите фамилию заказчика: " ANSI_RESET
#define WORK_TYPE_PROMPT ANSI_BLUE L"\nВведите тип работы: " ANSI_RESET
#define MIN_PRICE_PROMPT ANSI_BLUE L"\nВведите минимальную стоимость: " ANSI_RESET
#define MAX_PRICE_PROMPT ANSI_BLUE L"\nВведите максимальную стоимость: " ANSI_RESET
#define TEAM_PROMPT ANSI_BLUE L"\nВведите название команды: " ANSI_RESET

// Вспомогательная функция для отображения найденных заказов
static void printSearchResults(Order* orders, size_t count) {
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

// Поиск по фамилии
static bool searchByLastName() {
    wchar_t last_name[50] = {0};

    clearScreen();
    wprintf(L"%ls\n", SEARCH_TITLE);

    if (!getInput(LAST_NAME_PROMPT, last_name, 50)) {
        return false;
    }

    size_t count;
    Order* found_orders = findOrdersByLastName(last_name, &count);

    if (!found_orders || count == 0) {
        wprintf(NO_RESULTS_MSG);
    } else {
        wprintf(RESULTS_FOUND_MSG);
        printSearchResults(found_orders, count);

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

// Поиск по типу работы
static bool searchByWorkType() {
    wchar_t work_type[100] = {0};

    clearScreen();
    wprintf(L"%ls\n", SEARCH_TITLE);

    if (!getInput(WORK_TYPE_PROMPT, work_type, 100)) {
        return false;
    }

    size_t count;
    Order* found_orders = findOrdersByWorkType(work_type, &count);

    if (!found_orders || count == 0) {
        wprintf(NO_RESULTS_MSG);
    } else {
        wprintf(RESULTS_FOUND_MSG);
        printSearchResults(found_orders, count);

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

// Поиск по диапазону цен
static bool searchByPriceRange() {
    wchar_t min_price_str[20] = {0};
    wchar_t max_price_str[20] = {0};
    double min_price, max_price;

    clearScreen();
    wprintf(L"%ls\n", SEARCH_TITLE);

    // Получение минимальной цены
    while (true) {
        if (!getInput(MIN_PRICE_PROMPT, min_price_str, 20)) {
            return false;
        }

        if (!validatePrice(min_price_str)) {
            wprintf(INVALID_PRICE_INPUT_MSG);
            waitForKey();
            clearScreen();
            wprintf(L"%ls\n", SEARCH_TITLE);
            continue;
        }

        min_price = wcstod(min_price_str, NULL);
        break;
    }

    // Получение максимальной цены
    while (true) {
        if (!getInput(MAX_PRICE_PROMPT, max_price_str, 20)) {
            return false;
        }

        if (!validatePrice(max_price_str)) {
            wprintf(INVALID_PRICE_INPUT_MSG);
            waitForKey();
            clearScreen();
            wprintf(L"%ls\n", SEARCH_TITLE);
            wprintf(MIN_PRICE_PROMPT L"%ls\n", min_price_str);
            continue;
        }

        max_price = wcstod(max_price_str, NULL);
        if (max_price < min_price) {
            wprintf(ANSI_RED L"\nМаксимальная цена должна быть больше минимальной!\n" ANSI_RESET);
            waitForKey();
            clearScreen();
            wprintf(L"%ls\n", SEARCH_TITLE);
            wprintf(MIN_PRICE_PROMPT L"%ls\n", min_price_str);
            continue;
        }

        break;
    }

    size_t count;
    Order* found_orders = findOrdersByPriceRange(min_price, max_price, &count);

    if (!found_orders || count == 0) {
        wprintf(NO_RESULTS_MSG);
    } else {
        wprintf(RESULTS_FOUND_MSG);
        printSearchResults(found_orders, count);

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

// Поиск по команде
static bool searchByTeam() {
    wchar_t team[50] = {0};

    clearScreen();
    wprintf(L"%ls\n", SEARCH_TITLE);

    if (!getInput(TEAM_PROMPT, team, 50)) {
        return false;
    }

    size_t count;
    Order* found_orders = findOrdersByTeam(team, &count);

    if (!found_orders || count == 0) {
        wprintf(NO_RESULTS_MSG);
    } else {
        wprintf(RESULTS_FOUND_MSG);
        printSearchResults(found_orders, count);

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

bool searchOrdersDialog() {
    while (true) {
        clearScreen();
        wprintf(L"%ls\n", SEARCH_TITLE);
        wprintf(L"%ls\n\n", SEARCH_MENU_TITLE);
        
        wprintf(L"1. Поиск по фамилии заказчика\n");
        wprintf(L"2. Поиск по типу работы\n");
        wprintf(L"3. Поиск по диапазону стоимости\n");
        wprintf(L"4. Поиск по команде\n");
        wprintf(L"0. Назад\n\n");
        wprintf(L"Выберите действие: ");

        wint_t choice = _getwch();
        wprintf(L"%lc\n", choice);

        switch (choice) {
            case L'1':
                searchByLastName();
                break;
            case L'2':
                searchByWorkType();
                break;
            case L'3':
                searchByPriceRange();
                break;
            case L'4':
                searchByTeam();
                break;
            case L'0':
                return true;
            default:
                wprintf(L"\nНеверный выбор!\n");
                waitForKey();
        }
    }
} 