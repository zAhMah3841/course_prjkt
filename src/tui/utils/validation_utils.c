#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include "utils.h"

#define MIN_USERNAME_LEN 3
#define MAX_USERNAME_LEN 32
#define MIN_PASSWORD_LEN 8
#define VALID_USERNAME_CHARS L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-."

bool validatePassword(const wchar_t* password) {
    size_t len = wcslen(password);
    if (len < MIN_PASSWORD_LEN) {
        wprintf(L"Пароль должен содержать минимум %d символов\n", MIN_PASSWORD_LEN);
        return false;
    }

    bool has_upper = false, has_lower = false, has_digit = false;

    for (size_t i = 0; i < len; i++) {
        if (iswupper(password[i])) has_upper = true;
        if (iswlower(password[i])) has_lower = true;
        if (iswdigit(password[i])) has_digit = true;
    }

    if (!has_upper || !has_lower || !has_digit) {
        wprintf(L"Пароль должен содержать буквы разного регистра и цифры\n");
        return false;
    }

    return true;
}

bool isUsernameValid(const wchar_t* username) {
    const size_t len = wcslen(username);

    if (len < MIN_USERNAME_LEN || len > MAX_USERNAME_LEN) {
        wprintf(L"\nИмя пользователя должно быть от %d до %d символов!\n",
               MIN_USERNAME_LEN, MAX_USERNAME_LEN);
        return false;
    }

    for (size_t i = 0; i < len; i++) {
        if (!wcschr(VALID_USERNAME_CHARS, username[i])) {
            wprintf(L"\nИмя пользователя содержит недопустимые символы!\n"
                   L"Разрешены только: латинские буквы, цифры и символы _ - .\n");
            return false;
        }
    }

    if (iswdigit(username[0])) {
        wprintf(L"\nИмя пользователя не может начинаться с цифры!\n");
        return false;
    }

    return true;
}