#ifndef UNTITLED_LOGIN_H
#define UNTITLED_LOGIN_H

#include <wchar.h>

// Диалог входа в систему
// Возвращает:
// 1 - успешный вход
// 0 - отмена
// -1 - ошибка
int userLoginDialog(wchar_t* username, size_t username_size, unsigned max_attempts);

#endif //UNTITLED_LOGIN_H 