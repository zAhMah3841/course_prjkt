#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../../database/users_management/users_management.h"
#include "../tables/tables.h"
#include "dialogs.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define EDIT_USER_TITLE ANSI_CYAN L"=== Редактирование пользователя ===" ANSI_RESET
#define USERNAME_PROMPT ANSI_BLUE L"\nВведите логин пользователя для редактирования: " ANSI_RESET
#define NEW_PASSWORD_PROMPT ANSI_BLUE L"Введите новый пароль: " ANSI_RESET
#define CONFIRM_PASSWORD_PROMPT ANSI_BLUE L"Подтвердите новый пароль: " ANSI_RESET
#define ROLE_PROMPT ANSI_BLUE L"Выберите новую роль (1 - admin, 2 - user): " ANSI_RESET
#define SUCCESS_PASS_MSG ANSI_GREEN L"\nПароль успешно изменен!\n" ANSI_RESET
#define SUCCESS_ROLE_MSG ANSI_GREEN L"\nРоль успешно изменена!\n" ANSI_RESET
#define ERROR_PASS_MSG ANSI_RED L"\nОшибка при изменении пароля!\n" ANSI_RESET
#define ERROR_ROLE_MSG ANSI_RED L"\nОшибка при изменении роли!\n" ANSI_RESET
#define USER_NOT_FOUND_MSG ANSI_RED L"\nПользователь с таким логином не найден!\n" ANSI_RESET
#define PASS_MISMATCH_MSG ANSI_RED L"\nПароли не совпадают!\n" ANSI_RESET
#define INVALID_ROLE_MSG ANSI_RED L"\nНеверный выбор роли!\n" ANSI_RESET
#define EDIT_MENU_TITLE L"Редактирование пользователя: %ls"

bool changeUserPasswordDialog(const wchar_t* username) {
    wchar_t new_password[50] = {0};
    wchar_t confirm_password[50] = {0};

    while (true) {
        clearScreen();
        wprintf(L"%ls\n\n", EDIT_USER_TITLE);
        
        if (!getPassword(NEW_PASSWORD_PROMPT, new_password, 50)) {
            return false;
        }

        if (!validatePassword(new_password)) {
            waitForKey();
            continue;
        }

        if (!getPassword(CONFIRM_PASSWORD_PROMPT, confirm_password, 50)) {
            return false;
        }

        if (wcscmp(new_password, confirm_password) != 0) {
            wprintf(PASS_MISMATCH_MSG);
            waitForKey();
            continue;
        }

        if (changeUserPassword(username, new_password)) {
            wprintf(SUCCESS_PASS_MSG);
        } else {
            wprintf(ERROR_PASS_MSG);
        }

        waitForKey();
        return true;
    }
}

bool changeUserRoleDialog(const wchar_t* username) {
    wchar_t role_choice;
    const wchar_t* new_role;

    while (true) {
        clearScreen();
        wprintf(L"%ls\n\n", EDIT_USER_TITLE);
        wprintf(ROLE_PROMPT);
        
        role_choice = _getwch();
        wprintf(L"%lc\n", role_choice);

        if (role_choice == L'1') {
            new_role = L"admin";
            break;
        } else if (role_choice == L'2') {
            new_role = L"user";
            break;
        } else {
            wprintf(INVALID_ROLE_MSG);
            waitForKey();
        }
    }

    if (changeUserRole(username, new_role)) {
        wprintf(SUCCESS_ROLE_MSG);
    } else {
        wprintf(ERROR_ROLE_MSG);
    }

    waitForKey();
    return true;
}

void showEditUserMenu(const wchar_t* username) {
    while (true) {
        clearScreen();
        wchar_t menu_title[100];
        swprintf(menu_title, 100, EDIT_MENU_TITLE, username);
        wprintf(ANSI_CYAN L"=== %ls ===" ANSI_RESET L"\n\n", menu_title);
        
        wprintf(L"1. Изменить пароль\n");
        wprintf(L"2. Изменить роль\n");
        wprintf(L"0. Назад\n\n");
        wprintf(L"Выберите действие: ");

        wint_t choice = _getwch();
        wprintf(L"%lc\n", choice);

        switch (choice) {
            case L'1':
                changeUserPasswordDialog(username);
                break;
            case L'2':
                changeUserRoleDialog(username);
                break;
            case L'0':
                return;
            default:
                wprintf(L"\nНеверный выбор!\n");
                waitForKey();
        }
    }
}

bool editUserDialog() {
    wchar_t username[50] = {0};

    while (true) {
        clearScreen();
        wprintf(L"%ls\n\n", EDIT_USER_TITLE);

        // Показываем список пользователей
        printUsersTable();

        // Запрашиваем логин для редактирования
        if (!getInput(USERNAME_PROMPT, username, 50)) {
            return false;
        }

        // Проверяем существование пользователя
        if (!isUserExists(username)) {
            wprintf(USER_NOT_FOUND_MSG);
            waitForKey();
            continue;
        }

        // Показываем меню редактирования
        showEditUserMenu(username);
        return true;
    }
} 