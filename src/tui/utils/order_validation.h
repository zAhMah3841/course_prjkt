#ifndef ORDER_VALIDATION_H
#define ORDER_VALIDATION_H

#include <wchar.h>
#include <stdbool.h>

// Проверяет, что строка содержит корректное значение цены (положительное число)
bool validatePrice(const wchar_t* price_str);

#endif // ORDER_VALIDATION_H 