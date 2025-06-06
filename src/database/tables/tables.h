#ifndef TABLES_H
#define TABLES_H

#include <stdbool.h>

bool isTableExist(const char* table_name);
bool createDatabaseTable(const char* table_name, const char* fields);
int initDatabaseTables();

#endif //TABLES_H
