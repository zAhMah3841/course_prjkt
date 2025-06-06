#include "../common/database.h"
#include "../tui/utils/utils.h"
#include "users_management.h"
#include <stdlib.h>
#include <string.h>

bool isUserExists(const wchar_t* login) {
    const char* sql = "SELECT 1 FROM users WHERE login = ? LIMIT 1";
    sqlite3_stmt* stmt = prepareStatement(sql);

    bindTextParam(stmt, 1, wideToUtf8(login));
    const bool exists = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return exists;
}

User* getUsersList(size_t* count) {
    const char* sql = "SELECT login, password, role FROM users ORDER BY login";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return NULL;

    *count = getRecordsCount(stmt);
    User* users = (User*)calloc(*count, sizeof(User));
    if (!users) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        const unsigned char* login = sqlite3_column_text(stmt, 0);
        const unsigned char* password = sqlite3_column_text(stmt, 1);
        const unsigned char* role = sqlite3_column_text(stmt, 2);

        users[i].login = login ? strdup((const char*)login) : NULL;
        users[i].password = password ? strdup((const char*)password) : NULL;
        users[i].role = role ? strdup((const char*)role) : NULL;
        i++;
    }

    sqlite3_finalize(stmt);
    return users;
}

bool addUser(const User* user) {
    const char* sql = "INSERT INTO users (login, password, role) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt = prepareStatement(sql);

    bindTextParam(stmt, 1, user->login);
    bindTextParam(stmt, 2, user->password);
    bindTextParam(stmt, 3, user->role);

    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

bool deleteUser(const wchar_t* username) {
    const char* sql = "DELETE FROM users WHERE login = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);

    bindTextParam(stmt, 1, wideToUtf8(username));

    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

bool changeUserPassword(const wchar_t* username, const wchar_t* new_password) {
    const char* sql = "UPDATE users SET password = ? WHERE login = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);

    bindTextParam(stmt, 1, wideToUtf8(new_password));
    bindTextParam(stmt, 2, wideToUtf8(username));
    const bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

bool changeUserRole(const wchar_t* username, const wchar_t* new_role) {
    const char* sql = "UPDATE users SET role = ? WHERE login = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);

    bindTextParam(stmt, 1, wideToUtf8(new_role));
    bindTextParam(stmt, 2, wideToUtf8(username));
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

bool authenticateUser(const wchar_t* username, const wchar_t* password) {
    const char *sql = "SELECT 1 FROM users WHERE login= ? AND password= ?";
    sqlite3_stmt *stmt = prepareStatement(sql);
    bool result = false;

    bindTextParam(stmt, 1, wideToUtf8(username));
    bindTextParam(stmt, 2, wideToUtf8(password));

    if (sqlite3_step(stmt) == SQLITE_ROW) result = true;

    sqlite3_finalize(stmt);
    return result;
}

char* getUserRole(const wchar_t* username) {
    const char *sql = "SELECT role FROM users WHERE login= ?";
    sqlite3_stmt *stmt = prepareStatement(sql);
    char *role = NULL;

    bindTextParam(stmt, 1, wideToUtf8(username));

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *db_role = sqlite3_column_text(stmt, 0);
        if (db_role) {
            role = strdup((const char*)db_role);
        }
    }

    sqlite3_finalize(stmt);
    return role;
}