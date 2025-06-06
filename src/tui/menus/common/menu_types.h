#ifndef MENU_TYPES_H
#define MENU_TYPES_H

#include <wchar.h>

typedef union {
    int int_val;
    double double_val;
    char* str_val;
    void* ptr_val;
} MenuParam;

typedef void (*MenuHandler)(MenuParam param);

typedef struct {
    wchar_t key;
    const wchar_t* label;
    MenuHandler handler;
    MenuParam param;
} MenuItem;

typedef struct {
    const wchar_t* title;
    MenuItem* items;
    size_t count;
} Menu;

#endif //MENU_TYPES_H