#ifndef USERS_MANAGEMENT_H
#define USERS_MANAGEMENT_H

#include "../tables/schema.h"
#include <stdbool.h>
#include <wchar.h>

bool isUserExists(const wchar_t* login);

User* getUsersList(size_t* count);
char* getUserRole(const wchar_t* username);

bool addUser(const User* user);
bool deleteUser(const wchar_t* username);

bool changeUserPassword(const wchar_t* username, const wchar_t* new_password);
bool changeUserRole(const wchar_t* username, const wchar_t* new_role);
bool authenticateUser(const wchar_t* username, const wchar_t* password);

#endif //USERS_MANAGEMENT_H
