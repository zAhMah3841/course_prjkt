#ifndef SCHEMA_H
#define SCHEMA_H

#include <wchar.h>

typedef struct {
    char* login;
    char* password;
    char* role;
} User;

#define CREATE_USERS_TABLE \
    "login TEXT UNIQUE NOT NULL," \
    "password TEXT NOT NULL," \
    "role TEXT NOT NULL"

#define CREATE_REQUESTS_TABLE \
    "id INTEGER NOT NULL UNIQUE," \
    "name TEXT NOT NULL," \
    "surname TEXT NOT NULL, " \
    "lastname TEXT NOT NULL, " \
    "estimate REAL NOT NULL," \
    "work_type TEXT NOT NULL," \
    "team TEXT NOT NULL"

#endif //SCHEMA_H
