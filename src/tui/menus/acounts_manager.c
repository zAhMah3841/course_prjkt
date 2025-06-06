#include "../utils/utils.h"
#include "../utils/colors_definitions.h"
#include "../tables/tables.h"
#include "../dialogs/dialogs.h"
#include "menus.h"
#include <wchar.h>

// Глобальная переменная для хранения текущего пользователя
static wchar_t current_user[50];

void setCurrentUser(const wchar_t* username) {
    wcscpy(current_user, username);
}

void printUsersTableWrapper(MenuParam param) {
    printUsersTableAndWait();
}

void addUserDialogWrapper(MenuParam param) {
    addUserDialog();
}

void editUserDialogWrapper(MenuParam param) {
    editUserDialog();
}

void deleteUserDialogWrapper(MenuParam param) {
    deleteUserDialog(current_user);
}

#define ACCOUNTS_MANAGER_TITLE ANSI_CYAN L"=== Управление пользователями ===" ANSI_RESET

void runAccountsManagerMenu() {
    MenuItem menuItems[] = {
        {L'1', L"Просмотр списка пользователей", printUsersTableWrapper, {0}},
        {L'2', L"Добавить пользователя", addUserDialogWrapper, {0}},
        {L'3', L"Удалить пользователя", deleteUserDialogWrapper, {0}},
        {L'4', L"Редактировать пользователя", editUserDialogWrapper, {0}},
        {L'0', L"Назад", NULL, {0}}
    };

    Menu menu = {
        ACCOUNTS_MANAGER_TITLE,
        menuItems,
        sizeof(menuItems) / sizeof(MenuItem)
    };

    runMenu(&menu);
}