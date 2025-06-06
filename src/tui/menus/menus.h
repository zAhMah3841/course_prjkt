#ifndef MENUS_H
#define MENUS_H

#include <stdbool.h>
#include "common/menu_types.h"

MenuParam createIntParam(int value);
MenuParam createDoubleParam(double value);
MenuParam createStringParam(const char* value);

bool handleMenuChoice(const wint_t choice, const Menu* menu);
void runMenu(const Menu* menu);

void runUserMenu();

void runAdminMenu();
void runAccountsManagerMenu();
void runOrdersManagerMenu();

#endif //MENUS_H
