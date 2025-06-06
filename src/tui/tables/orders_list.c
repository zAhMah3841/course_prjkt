#include <stdlib.h>
#include "tables.h"
#include "../utils/utils.h"
#include "../widgets/table/table.h"
#include "../../database/tables/schema.h"
#include "../../database/orders_management/orders_management.h"
#include <wchar.h>
#include "../utils/colors_definitions.h"

void printOrdersTable() {
    size_t count;
    Order* orders = getOrdersList(&count);

    if (!orders) {
        wprintf(L"Ошибка при получении списка заказов\n");
        return;
    }

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

    // Освобождаем память заказов
    for (size_t i = 0; i < count; i++) {
        free(orders[i].last_name);
        free(orders[i].first_name);
        free(orders[i].surname_name);
        free(orders[i].work_type);
        free(orders[i].team);
    }
    free(orders);
}

void printOrdersTableAndWait() {
    printOrdersTable();
    waitForKey();
} 