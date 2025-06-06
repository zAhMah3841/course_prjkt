#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "tui/utils/utils.h"
#include "tui/dialogs/dialogs.h"
#include "database/common/database.h"
#include "database/users_management/users_management.h"
#include "loger/loger.h"
#include "tui/menus/menus.h"
#include "tui/tables/tables.h"

// Объявляем функцию из accounts_manager.c
void setCurrentUser(const wchar_t* username);

int main() {
    initConsole();

    if (initDatabase("database.db") != SQLITE_OK) {
        LOG_ERROR(sqlite3_errmsg(db));
        closeDatabase();
        return 1;
    }

    wchar_t username[50];

    if (userLoginDialog(username, 50, 3) == 1) {
        setCurrentUser(username); // Устанавливаем текущего пользователя
        while(strcmp(getUserRole(username), "admin") != 0 && strcmp(getUserRole(username), "user") != 0) {
            wprintf(L"Ошибка: у данного пользователя неверная роль, войдите с другой учетной записи!");
            getch();
            if (userLoginDialog(username, 50, 3) == 1) {
                setCurrentUser(username);
            }
        }
        if (strcmp(getUserRole(username), "admin") == 0) {
            runAdminMenu();
        } else if (strcmp(getUserRole(username), "user") == 0) {
            runUserMenu();
        }
    }

    closeDatabase();
    return 0;
}
