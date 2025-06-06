#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include <stdbool.h>
#include <stddef.h>

extern sqlite3* db;

bool initializeDatabase(const char* dbPath);
int createUsersTable();
bool isAdminExist();
int createBasicAdmin();
int createOrdersTable();
int initDatabase(const char* db_path);
void closeDatabase();
sqlite3_stmt* prepareStatement(const char* sql);
int bindTextParam(sqlite3_stmt* stmt, int index, const char* value);
int bindIntParam(sqlite3_stmt* stmt, int index, int value);
int bindRealParam(sqlite3_stmt* stmt, int index, double value);
size_t getRecordsCount(sqlite3_stmt* stmt);

#endif // DATABASE_H
