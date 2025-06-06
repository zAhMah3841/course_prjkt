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

#define SORT_TITLE ANSI_CYAN L"=== Сортировка заказов ===" ANSI_RESET
#define SORT_MENU_TITLE L"\nВыберите критерий сортировки:"
#define NO_ORDERS_MSG ANSI_YELLOW L"\nНет заказов для отображения.\n" ANSI_RESET
#define SORTED_MSG ANSI_GREEN L"\nЗаказы отсортированы:\n" ANSI_RESET

// Вспомогательная функция для отображения отсортированных заказов
static void printSortedResults(Order* orders, size_t count) {
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

// Функция для освобождения памяти заказов
static void freeOrders(Order* orders, size_t count) {
    if (!orders) return;
    
    for (size_t i = 0; i < count; i++) {
        free(orders[i].last_name);
        free(orders[i].first_name);
        free(orders[i].surname_name);
        free(orders[i].work_type);
        free(orders[i].team);
    }
    free(orders);
}

// Сортировка по номеру
static bool sortByNumber() {
    size_t count;
    Order* sorted_orders = getOrdersSortedByNumber(&count);

    if (!sorted_orders || count == 0) {
        wprintf(NO_ORDERS_MSG);
    } else {
        wprintf(SORTED_MSG);
        printSortedResults(sorted_orders, count);
        freeOrders(sorted_orders, count);
    }

    waitForKey();
    return true;
}

// Сортировка по фамилии
static bool sortByLastName() {
    size_t count;
    Order* sorted_orders = getOrdersSortedByLastName(&count);

    if (!sorted_orders || count == 0) {
        wprintf(NO_ORDERS_MSG);
    } else {
        wprintf(SORTED_MSG);
        printSortedResults(sorted_orders, count);
        freeOrders(sorted_orders, count);
    }

    waitForKey();
    return true;
}

// Сортировка по типу работы
static bool sortByWorkType() {
    size_t count;
    Order* sorted_orders = getOrdersSortedByWorkType(&count);

    if (!sorted_orders || count == 0) {
        wprintf(NO_ORDERS_MSG);
    } else {
        wprintf(SORTED_MSG);
        printSortedResults(sorted_orders, count);
        freeOrders(sorted_orders, count);
    }

    waitForKey();
    return true;
}

// Сортировка по стоимости
static bool sortByPrice() {
    size_t count;
    Order* sorted_orders = getOrdersSortedByPrice(&count);

    if (!sorted_orders || count == 0) {
        wprintf(NO_ORDERS_MSG);
    } else {
        wprintf(SORTED_MSG);
        printSortedResults(sorted_orders, count);
        freeOrders(sorted_orders, count);
    }

    waitForKey();
    return true;
}

// Сортировка по команде
static bool sortByTeam() {
    size_t count;
    Order* sorted_orders = getOrdersSortedByTeam(&count);

    if (!sorted_orders || count == 0) {
        wprintf(NO_ORDERS_MSG);
    } else {
        wprintf(SORTED_MSG);
        printSortedResults(sorted_orders, count);
        freeOrders(sorted_orders, count);
    }

    waitForKey();
    return true;
}

bool sortOrdersDialog() {
    while (true) {
        clearScreen();
        wprintf(L"%ls\n", SORT_TITLE);
        wprintf(L"%ls\n\n", SORT_MENU_TITLE);
        
        wprintf(L"1. Сортировка по номеру заказа\n");
        wprintf(L"2. Сортировка по фамилии заказчика\n");
        wprintf(L"3. Сортировка по типу работы\n");
        wprintf(L"4. Сортировка по стоимости\n");
        wprintf(L"5. Сортировка по команде\n");
        wprintf(L"0. Назад\n\n");
        wprintf(L"Выберите действие: ");

        wint_t choice = _getwch();
        wprintf(L"%lc\n", choice);

        switch (choice) {
            case L'1':
                sortByNumber();
                break;
            case L'2':
                sortByLastName();
                break;
            case L'3':
                sortByWorkType();
                break;
            case L'4':
                sortByPrice();
                break;
            case L'5':
                sortByTeam();
                break;
            case L'0':
                return true;
            default:
                wprintf(L"\nНеверный выбор!\n");
                waitForKey();
        }
    }
} 