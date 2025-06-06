#include "../common/database.h"
#include "../../loger/loger.h"
#include "schema.h"
#include "tables.h"
#include <stdio.h>

bool isTableExist(const char* table_name) {
    const char* sql = "SELECT 1 FROM sqlite_master WHERE type='table' AND name=? LIMIT 1";
    sqlite3_stmt* stmt = prepareStatement(sql);

    bindTextParam(stmt, 1, table_name);
    const bool result = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return result;
}

bool createDatabaseTable(const char* table_name, const char* fields) {
    char sql[1024];
    sprintf(sql, "CREATE TABLE %s(%s)", table_name, fields);

    if (sqlite3_exec(db, sql, 0, 0, NULL) != SQLITE_OK) return false;
    return true;
}

int createTableIfNotExist(const char* table_name, const char* table_fields,
    const char* warning_msg, const char* error_msg, const char* success_msg) {
    if (!isTableExist(table_name)) {
        LOG_INFO(warning_msg);

        if (!createDatabaseTable(table_name, table_fields)) {
            LOG_ERROR(error_msg);
            return 1;
        }

        LOG_INFO(success_msg);
    }

    return 0;
}

int createUsersTable() {
    return createTableIfNotExist("users", CREATE_USERS_TABLE,
    "Таблица пользователей не существует. Создаем... \n",
    "Ошибка: Не удалось создать таблицу пользователей \n",
    "Таблица пользователей успешно создана \n");
}

int createOrdersTable() {
    return createTableIfNotExist("orders", CREATE_REQUESTS_TABLE,
        "Таблица заказов не существует. Создаем... \n",
        "Ошибка: Не удалось создать таблицу заказов \n",
        "Таблица заказов успешно создана \n");
}


int initDatabaseTables(){
    return createUsersTable() | createOrdersTable();
}