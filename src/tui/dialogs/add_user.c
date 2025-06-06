//
// Created by Gleb on 03.06.2025.
//

#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../../database/users_management/users_management.h"
#include "dialogs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define ADD_USER_TITLE ANSI_CYAN L"=== Добавление нового пользователя ===" ANSI_RESET
#define USERNAME_PROMPT ANSI_BLUE L"\nВведите логин: " ANSI_RESET
#define PASSWORD_PROMPT ANSI_BLUE L"Введите пароль: " ANSI_RESET
#define ROLE_PROMPT ANSI_BLUE L"Выберите роль (1 - admin, 2 - user): " ANSI_RESET
#define SUCCESS_MSG ANSI_GREEN L"\nПользователь успешно добавлен!\n" ANSI_RESET
#define ERROR_MSG ANSI_RED L"\nОшибка при добавлении пользователя!\n" ANSI_RESET
#define INVALID_ROLE_MSG ANSI_RED L"\nНеверный номер роли! Выберите 1 или 2.\n" ANSI_RESET

const char* getRoleById(int roleId) {
    switch (roleId) {
        case 1:
            return "admin";
        case 2:
            return "user";
        default:
            return NULL;
    }
}

bool validateRole(const wchar_t* role_str) {
    wchar_t* end;
    long value = wcstol(role_str, &end, 10);
    
    // Проверяем, что строка полностью преобразована в число
    if (*end != L'\0') {
        return false;
    }
    
    // Проверяем диапазон
    if (value < 1 || value > 2) {
        return false;
    }
    
    return true;
}

bool addUserDialog() {
    wchar_t username[50] = {0};
    wchar_t password[50] = {0};
    wchar_t role_str[10] = {0};
    const char* role = NULL;

    clearScreen();
    wprintf(L"%ls\n", ADD_USER_TITLE);

    // Получение логина
    if (!getInput(USERNAME_PROMPT, username, 50)) {
        return false;
    }

    // Получение пароля
    if (!getInput(PASSWORD_PROMPT, password, 50)) {
        return false;
    }

    // Получение и валидация роли
    while (true) {
        if (!getInput(ROLE_PROMPT, role_str, 10)) {
            return false;
        }

        if (!validateRole(role_str)) {
            wprintf(INVALID_ROLE_MSG);
            waitForKey();
            clearScreen();
            wprintf(L"%ls\n", ADD_USER_TITLE);
            wprintf(USERNAME_PROMPT L"%ls\n", username);
            wprintf(PASSWORD_PROMPT L"%ls\n", password);
            continue;
        }

        role = getRoleById(wcstol(role_str, NULL, 10));
        break;
    }

    // Создание структуры пользователя
    User new_user = {
        .login = wideToUtf8(username),
        .password = wideToUtf8(password),
        .role = strdup(role)
    };

    // Добавление пользователя в базу данных
    bool success = addUser(&new_user);

    // Освобождение памяти
    free(new_user.login);
    free(new_user.password);
    free(new_user.role);

    if (success) {
        wprintf(SUCCESS_MSG);
    } else {
        wprintf(ERROR_MSG);
    }

    waitForKey();
    return success;
}
