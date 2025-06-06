#include "order_validation.h"

bool validatePrice(const wchar_t* price_str) {
    wchar_t* end;
    double value = wcstod(price_str, &end);
    
    // Проверяем, что строка полностью преобразована в число
    if (*end != L'\0') {
        return false;
    }
    
    // Проверяем, что цена положительная
    if (value <= 0.0) {
        return false;
    }
    
    return true;
} 