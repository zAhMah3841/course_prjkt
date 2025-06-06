#ifndef MENU_COMPONENTS_H
#define MENU_COMPONENTS_H

#include "../menus.h"
#include "../../utils/layout_utils.h"

// Стили для различных типов меню
typedef struct {
    const wchar_t* title_prefix;  // Префикс заголовка (например, "МЕНЮ" или "УПРАВЛЕНИЕ")
    int min_width;                // Минимальная ширина меню
    int padding_x;                // Горизонтальные отступы
    int padding_y;                // Вертикальные отступы
    int spacing;                  // Расстояние между пунктами
} MenuStyle;

// Предопределенные стили меню
extern const MenuStyle MAIN_MENU_STYLE;      // Для главного меню
extern const MenuStyle ADMIN_MENU_STYLE;     // Для меню администратора
extern const MenuStyle USER_MENU_STYLE;      // Для меню пользователя
extern const MenuStyle MANAGER_MENU_STYLE;   // Для меню управления

// Функции для отрисовки компонентов меню
void drawMenuTitle(const wchar_t* title, const MenuStyle* style, BoxBounds* bounds);
void drawMenuItem(const MenuItem* item, int index, const MenuStyle* style, BoxBounds* bounds);
void drawMenuFooter(const MenuStyle* style, BoxBounds* bounds);
void drawMenuPrompt(const wchar_t* prompt, const MenuStyle* style, BoxBounds* bounds);

// Функция для расчета размеров меню
BoxBounds calculateMenuBounds(const Menu* menu, const MenuStyle* style);

#endif // MENU_COMPONENTS_H 