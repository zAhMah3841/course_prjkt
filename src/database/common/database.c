#include "database.h"
#include <stddef.h>
#include "loger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

static sqlite3* db_instance = NULL;
sqlite3* db = NULL;
char* errMsg = NULL;

bool initializeDatabase(const char* dbPath) {
    if (sqlite3_open(dbPath, &db) != SQLITE_OK) {
        fprintf(stderr, "[ERROR] Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }

    if (createUsersTable())
        return 1;

    if (createBasicAdmin())
        return 1;

    if (createOrdersTable())
        return 1;

    return true;
}

// Создание таблицы пользователей, при ее отсутствии
int createUsersTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                      "login TEXT UNIQUE,"
                      "password TEXT,"
                      "role TEXT"
                      ");";

    if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "[ERROR] Failed to create users table: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    }
    return 0;
}

// Проверка на существования аккаунта администратора
bool isAdminExist() {
    sqlite3_stmt* res;
    const char* isExistAdmin = "SELECT 1 FROM users WHERE role='admin'";

    if (sqlite3_prepare_v2(db, isExistAdmin, -1, &res, NULL) == SQLITE_OK) {
        if (sqlite3_step(res) == SQLITE_ROW) {
            if (sqlite3_column_int(res, 0)) {
                sqlite3_finalize(res);
                return true;
            }
        }
    }
    else {
        fprintf(stderr, "[ERROR] Can't check is admin exist: %s\n", sqlite3_errmsg(db));
        getch();
        return false;
    }

    wprintf(L"Не существует ни одного администратора, будет создан администратор со следующими данными\n"
                  L"Логин: admin\n"
                  L"Пароль: 123\n");
    _getch();
    sqlite3_finalize(res);
    return false;
}

// Создание аккаунта администратора, если нет ни одного пользователя-администратора
int createBasicAdmin() {
    if (!isAdminExist()) {
        const char* sql = "INSERT INTO users("
                          "login, password, role)"
                          "SELECT 'admin', '123', 'admin'";

        if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK) {
                fprintf(stderr, "[ERROR] Failed to create basic admin: %s\n", errMsg);
                sqlite3_free(errMsg);
                return 1;
        }
    }
    return 0;
}

// Создание таблицы заказов при ее отсутствии
int createOrdersTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS orders ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "last_name TEXT,"
                      "first_name TEXT,"
                      "surname_name TEXT,"
                      "work_type TEXT,"
                      "price REAL,"
                      "team TEXT"
                      ");";

    if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "[ERROR] Failed to create orders table: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    }
    return 0;
}

int initDatabase(const char* db_path) {
    if (!initializeDatabase(db_path)) {
        return 1;
    }
    return 0;
}

void closeDatabase(void) {
    if (db_instance) {
        sqlite3_close(db_instance);
        db_instance = NULL;
        db = NULL;
    }
}

sqlite3_stmt* prepareStatement(const char* sql) {
    sqlite3_stmt* stmt = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_ERROR(sqlite3_errmsg(db));
        return NULL;
    }

    return stmt;
}

int bindTextParam(sqlite3_stmt* stmt, int index, const char* value) {
    int rc = sqlite3_bind_text(stmt, index, value, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        LOG_ERROR(sqlite3_errmsg(db));
    }
    return rc;
}

int bindIntParam(sqlite3_stmt* stmt, int index, int value) {
    int rc = sqlite3_bind_int(stmt, index, value);
    if (rc != SQLITE_OK) {
        LOG_ERROR(sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
    return rc;
}

int bindRealParam(sqlite3_stmt* stmt, int index, double value) {
    int rc = sqlite3_bind_double(stmt, index, value);
    if (rc != SQLITE_OK) {
        LOG_ERROR(sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
    return rc;
}

size_t getRecordsCount(sqlite3_stmt* stmt) {
    size_t count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) count++;

    sqlite3_reset(stmt);
    return count;
}