#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../utils/order_validation.h"
#include "../../database/orders_management/orders_management.h"
#include "dialogs.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define ADD_ORDER_TITLE ANSI_CYAN L"=== Добавление нового заказа ===" ANSI_RESET
#define LAST_NAME_PROMPT ANSI_BLUE L"\nВведите фамилию заказчика: " ANSI_RESET
#define FIRST_NAME_PROMPT ANSI_BLUE L"Введите имя заказчика: " ANSI_RESET
#define SURNAME_PROMPT ANSI_BLUE L"Введите отчество заказчика: " ANSI_RESET
#define WORK_TYPE_PROMPT ANSI_BLUE L"Введите тип работы: " ANSI_RESET
#define PRICE_PROMPT ANSI_BLUE L"Введите стоимость работы: " ANSI_RESET
#define TEAM_PROMPT ANSI_BLUE L"Введите название команды: " ANSI_RESET
#define SUCCESS_MSG ANSI_GREEN L"\nЗаказ успешно добавлен!\n" ANSI_RESET
#define ERROR_MSG ANSI_RED L"\nОшибка при добавлении заказа!\n" ANSI_RESET
#define INVALID_PRICE_MSG ANSI_RED L"\nНеверное значение стоимости! Введите положительное число.\n" ANSI_RESET

bool addOrderDialog() {
    wchar_t last_name[50] = {0};
    wchar_t first_name[50] = {0};
    wchar_t surname[50] = {0};
    wchar_t work_type[100] = {0};
    wchar_t price_str[20] = {0};
    wchar_t team[50] = {0};
    double price;

    clearScreen();
    wprintf(L"%ls\n", ADD_ORDER_TITLE);

    // Получение фамилии
    if (!getInput(LAST_NAME_PROMPT, last_name, 50)) {
        return false;
    }

    // Получение имени
    if (!getInput(FIRST_NAME_PROMPT, first_name, 50)) {
        return false;
    }

    // Получение отчества
    if (!getInput(SURNAME_PROMPT, surname, 50)) {
        return false;
    }

    // Получение типа работы
    if (!getInput(WORK_TYPE_PROMPT, work_type, 100)) {
        return false;
    }

    // Получение и валидация цены
    while (true) {
        if (!getInput(PRICE_PROMPT, price_str, 20)) {
            return false;
        }

        if (!validatePrice(price_str)) {
            wprintf(INVALID_PRICE_MSG);
            waitForKey();
            clearScreen();
            wprintf(L"%ls\n", ADD_ORDER_TITLE);
            wprintf(LAST_NAME_PROMPT L"%ls\n", last_name);
            wprintf(FIRST_NAME_PROMPT L"%ls\n", first_name);
            wprintf(SURNAME_PROMPT L"%ls\n", surname);
            wprintf(WORK_TYPE_PROMPT L"%ls\n", work_type);
            continue;
        }

        price = wcstod(price_str, NULL);
        break;
    }

    // Получение названия команды
    if (!getInput(TEAM_PROMPT, team, 50)) {
        return false;
    }

    // Создание структуры заказа
    Order order = {
        .last_name = last_name,
        .first_name = first_name,
        .surname_name = surname,
        .work_type = work_type,
        .price = price,
        .team = team
    };

    // Добавление заказа в базу данных
    if (addOrder(&order)) {
        wprintf(SUCCESS_MSG);
    } else {
        wprintf(ERROR_MSG);
    }

    waitForKey();
    return true;
} 