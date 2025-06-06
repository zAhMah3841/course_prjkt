#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../../database/users_management/users_management.h"
#include "../tables/tables.h"
#include "dialogs.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define DELETE_USER_TITLE ANSI_CYAN L"=== Удаление пользователя ===" ANSI_RESET
#define USERNAME_PROMPT ANSI_BLUE L"\nВведите логин пользователя для удаления: " ANSI_RESET
#define CONFIRM_DELETE_MSG L"Вы действительно хотите удалить пользователя '%ls'?"
#define SUCCESS_MSG ANSI_GREEN L"\nПользователь успешно удален!\n" ANSI_RESET
#define ERROR_MSG ANSI_RED L"\nОшибка при удалении пользователя!\n" ANSI_RESET
#define USER_NOT_FOUND_MSG ANSI_RED L"\nПользователь с таким логином не найден!\n" ANSI_RESET
#define SELF_DELETE_MSG ANSI_RED L"\nВы не можете удалить свой собственный аккаунт!\n" ANSI_RESET

bool deleteUserDialog(const wchar_t* current_user) {
    wchar_t username[50] = {0};

    while (true) {
        clearScreen();
        wprintf(L"%ls\n\n", DELETE_USER_TITLE);

        // Показываем список пользователей
        printUsersTable();

        // Запрашиваем логин для удаления
        if (!getInput(USERNAME_PROMPT, username, 50)) {
            return false;
        }

        // Проверяем, не пытается ли пользователь удалить свой аккаунт
        if (wcscmp(username, current_user) == 0) {
            wprintf(SELF_DELETE_MSG);
            waitForKey();
            continue;
        }

        // Проверяем существование пользователя
        if (!isUserExists(username)) {
            wprintf(USER_NOT_FOUND_MSG);
            waitForKey();
            continue;
        }

        // Запрашиваем подтверждение
        wchar_t confirm_msg[256];
        swprintf(confirm_msg, 256, CONFIRM_DELETE_MSG, username);
        
        if (!confirmAction(confirm_msg, false)) {
            return false;
        }

        // Удаляем пользователя
        if (deleteUser(username)) {
            wprintf(SUCCESS_MSG);
        } else {
            wprintf(ERROR_MSG);
        }

        waitForKey();
        return true;
    }
} 