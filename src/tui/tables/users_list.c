#include <stdlib.h>
#include "tables.h"
#include "../utils/utils.h"
#include "../widgets/table/table.h"
#include "../../database/tables/schema.h"
#include "../../database/users_management/users_management.h"

void printUsersTable() {
    size_t userCount = 0;
    const User* users = getUsersList(&userCount);

    clearScreen();
    wprintf(L"=== СПИСОК ПОЛЬЗОВАТЕЛЕЙ ===\n\n");
    if (userCount <= 0) {
        wprintf(L"Ошибка при получении списка пользователей или список пуст.\n");
        return;
    }

    Table* table = createTable(userCount + 1, 2);

    setCell(table, 0, 0, L"Логин");
    setCell(table, 0, 1, L"Роль");

    for (int i = 0; i < userCount; i++) {
        setCell(table, i + 1, 0, utf8ToWide(users[i].login));
        setCell(table, i + 1, 1, utf8ToWide(users[i].role));
    }

    table->col_wrap[0] = false;
    table->col_wrap[1] = true;

    calculateLayout(table, 50);
    printTable(table);

    freeTable(table);

    // Free memory properly
    for (size_t i = 0; i < userCount; i++) {
        free(users[i].login);
        free(users[i].password);
        free(users[i].role);
    }
    free((void*)users);
}

void printUsersTableAndWait() {
    printUsersTable();
    waitForKey();
}