#include "../database/users_management/users_management.h"
#include "../utils/colors_definitions.h"
#include "../utils/utils.h"
#include "dialogs.h"
#include <signal.h>
#include <stdio.h>
#include <wchar.h>

#define LOGIN_TITLE ANSI_CYAN L"=== Система входа ===" ANSI_RESET
#define LOGIN_PROMPT ANSI_BLUE L"Логин: " ANSI_RESET
#define PASS_PROMPT  ANSI_BLUE L"Пароль: " ANSI_RESET
#define LOGIN_SUCCESS ANSI_GREEN L"\nВход выполнен! Добро пожаловать, %ls!\n" ANSI_RESET
#define LOGIN_ERROR ANSI_RED L"\nОшибка! Неверный логин или пароль.\n" ANSI_RESET
#define MAX_ATTEMPTS_MSG ANSI_BRIGHT_RED L"\nДоступ запрещен! Исчерпаны все попытки.\n" ANSI_RESET
#define ATTEMPTS_LEFT ANSI_YELLOW L"Попыток осталось: %d\n" ANSI_RESET
#define EXIT_MSG ANSI_YELLOW L"\nЗавершение программы...\n" ANSI_RESET


int userLoginDialog(wchar_t* output_username, size_t username_size, unsigned max_attempts) {
    wchar_t login[50] = {0};
    wchar_t password[50] = {0};
    unsigned attempts = 0;

    signal(SIGINT, handleSignal);

    while (!exit_flag && (max_attempts == 0 || attempts < max_attempts)) {
        clearScreen();
        wprintf(L"%ls\n\n", LOGIN_TITLE);

        if (!getInput(LOGIN_PROMPT, login, 50)) {
            wprintf(EXIT_MSG);
            return -1;
        }

        if (!getPassword(PASS_PROMPT, password, 50)) {
            wprintf(EXIT_MSG);
            return -1;
        }

        if (authenticateUser(login, password)) {
            wprintf(LOGIN_SUCCESS, login);
            waitForKey();

            wcsncpy(output_username, login, username_size - 1);
            output_username[username_size - 1] = L'\0';
            return 1;
        }

        attempts++;
        wprintf(LOGIN_ERROR);

        if (max_attempts > 0 && attempts >= max_attempts) {
            wprintf(MAX_ATTEMPTS_MSG);
            waitForKey();
            return 0;
        }

        wprintf(ATTEMPTS_LEFT, max_attempts - attempts);
        waitForKey();
    }

    return 0;
}