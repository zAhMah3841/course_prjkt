#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../utils/order_validation.h"
#include "../../database/orders_management/orders_management.h"
#include "../tables/tables.h"
#include "dialogs.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define EDIT_ORDER_TITLE ANSI_CYAN L"=== Редактирование заказа ===" ANSI_RESET
#define ORDER_NUMBER_PROMPT ANSI_BLUE L"\nВведите номер заказа для редактирования: " ANSI_RESET
#define NEW_LAST_NAME_PROMPT ANSI_BLUE L"Введите новую фамилию заказчика: " ANSI_RESET
#define NEW_FIRST_NAME_PROMPT ANSI_BLUE L"Введите новое имя заказчика: " ANSI_RESET
#define NEW_SURNAME_PROMPT ANSI_BLUE L"Введите новое отчество заказчика: " ANSI_RESET
#define NEW_WORK_TYPE_PROMPT ANSI_BLUE L"Введите новый тип работы: " ANSI_RESET
#define NEW_PRICE_PROMPT ANSI_BLUE L"Введите новую стоимость работы: " ANSI_RESET
#define NEW_TEAM_PROMPT ANSI_BLUE L"Введите новое название команды: " ANSI_RESET
#define SUCCESS_MSG ANSI_GREEN L"\nЗаказ успешно обновлен!\n" ANSI_RESET
#define ERROR_MSG ANSI_RED L"\nОшибка при обновлении заказа!\n" ANSI_RESET
#define ORDER_NOT_FOUND_MSG ANSI_RED L"\nЗаказ с таким номером не найден!\n" ANSI_RESET
#define INVALID_PRICE_MSG ANSI_RED L"\nНеверное значение стоимости! Введите положительное число.\n" ANSI_RESET
#define EDIT_MENU_TITLE L"Редактирование заказа №%d"

bool changeOrderCustomerDialog(Order* order) {
    wchar_t last_name[50] = {0};
    wchar_t first_name[50] = {0};
    wchar_t surname[50] = {0};

    clearScreen();
    wprintf(L"%ls\n", EDIT_ORDER_TITLE);

    // Получение новой фамилии
    if (!getInput(NEW_LAST_NAME_PROMPT, last_name, 50)) {
        return false;
    }

    // Получение нового имени
    if (!getInput(NEW_FIRST_NAME_PROMPT, first_name, 50)) {
        return false;
    }

    // Получение нового отчества
    if (!getInput(NEW_SURNAME_PROMPT, surname, 50)) {
        return false;
    }

    // Обновляем данные заказа
    free(order->last_name);
    free(order->first_name);
    free(order->surname_name);
    
    order->last_name = _wcsdup(last_name);
    order->first_name = _wcsdup(first_name);
    order->surname_name = _wcsdup(surname);

    if (updateOrder(order)) {
        wprintf(SUCCESS_MSG);
    } else {
        wprintf(ERROR_MSG);
    }

    waitForKey();
    return true;
}

bool changeOrderWorkDialog(Order* order) {
    wchar_t work_type[100] = {0};
    wchar_t price_str[20] = {0};
    double price;

    clearScreen();
    wprintf(L"%ls\n", EDIT_ORDER_TITLE);

    // Получение нового типа работы
    if (!getInput(NEW_WORK_TYPE_PROMPT, work_type, 100)) {
        return false;
    }

    // Получение и валидация новой цены
    while (true) {
        if (!getInput(NEW_PRICE_PROMPT, price_str, 20)) {
            return false;
        }

        if (!validatePrice(price_str)) {
            wprintf(INVALID_PRICE_MSG);
            waitForKey();
            clearScreen();
            wprintf(L"%ls\n", EDIT_ORDER_TITLE);
            wprintf(NEW_WORK_TYPE_PROMPT L"%ls\n", work_type);
            continue;
        }

        price = wcstod(price_str, NULL);
        break;
    }

    // Обновляем данные заказа
    free(order->work_type);
    order->work_type = _wcsdup(work_type);
    order->price = price;

    if (updateOrder(order)) {
        wprintf(SUCCESS_MSG);
    } else {
        wprintf(ERROR_MSG);
    }

    waitForKey();
    return true;
}

bool changeOrderTeamDialog(Order* order) {
    wchar_t team[50] = {0};

    clearScreen();
    wprintf(L"%ls\n", EDIT_ORDER_TITLE);

    // Получение нового названия команды
    if (!getInput(NEW_TEAM_PROMPT, team, 50)) {
        return false;
    }

    // Обновляем данные заказа
    free(order->team);
    order->team = _wcsdup(team);

    if (updateOrder(order)) {
        wprintf(SUCCESS_MSG);
    } else {
        wprintf(ERROR_MSG);
    }

    waitForKey();
    return true;
}

void showEditOrderMenu(Order* order) {
    while (true) {
        clearScreen();
        wchar_t menu_title[100];
        swprintf(menu_title, 100, EDIT_MENU_TITLE, order->number);
        wprintf(ANSI_CYAN L"=== %ls ===" ANSI_RESET L"\n\n", menu_title);
        
        wprintf(L"1. Изменить данные заказчика\n");
        wprintf(L"2. Изменить данные о работе\n");
        wprintf(L"3. Изменить команду\n");
        wprintf(L"0. Назад\n\n");
        wprintf(L"Выберите действие: ");

        wint_t choice = _getwch();
        wprintf(L"%lc\n", choice);

        switch (choice) {
            case L'1':
                changeOrderCustomerDialog(order);
                break;
            case L'2':
                changeOrderWorkDialog(order);
                break;
            case L'3':
                changeOrderTeamDialog(order);
                break;
            case L'0':
                return;
            default:
                wprintf(L"\nНеверный выбор!\n");
                waitForKey();
        }
    }
}

bool editOrderDialog() {
    wchar_t number_str[10] = {0};
    int number;

    while (true) {
        clearScreen();
        wprintf(L"%ls\n\n", EDIT_ORDER_TITLE);

        // Показываем список заказов
        printOrdersTable();

        // Запрашиваем номер заказа для редактирования
        if (!getInput(ORDER_NUMBER_PROMPT, number_str, 10)) {
            return false;
        }

        // Преобразуем строку в число
        wchar_t* end;
        number = wcstol(number_str, &end, 10);
        if (*end != L'\0') {
            wprintf(L"\nНеверный формат номера!\n");
            waitForKey();
            continue;
        }

        // Проверяем существование заказа
        if (!isOrderExists(number)) {
            wprintf(ORDER_NOT_FOUND_MSG);
            waitForKey();
            continue;
        }

        // Получаем данные заказа
        size_t count;
        Order* orders = getOrdersList(&count);
        if (!orders) {
            wprintf(ERROR_MSG);
            waitForKey();
            return false;
        }

        // Находим нужный заказ
        Order* order = NULL;
        for (size_t i = 0; i < count; i++) {
            if (orders[i].number == number) {
                order = &orders[i];
                break;
            }
        }

        if (!order) {
            wprintf(ERROR_MSG);
            // Освобождаем память
            for (size_t i = 0; i < count; i++) {
                free(orders[i].last_name);
                free(orders[i].first_name);
                free(orders[i].surname_name);
                free(orders[i].work_type);
                free(orders[i].team);
            }
            free(orders);
            waitForKey();
            return false;
        }

        // Показываем меню редактирования
        showEditOrderMenu(order);

        // Освобождаем память
        for (size_t i = 0; i < count; i++) {
            if (i != (order - orders)) { // Пропускаем редактируемый заказ, так как он может быть еще нужен
                free(orders[i].last_name);
                free(orders[i].first_name);
                free(orders[i].surname_name);
                free(orders[i].work_type);
                free(orders[i].team);
            }
        }
        free(orders);
        return true;
    }
} 